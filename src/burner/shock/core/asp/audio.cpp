
// See License.md for license

#include "../../includes.h"

int             Audio::mSamplesPerTick;
char            Audio::mReadBuffer[ MAX_AUDIO_BUFFER_BYTES ];
int             Audio::mDspHandle;
RingBuffer      Audio::mDspRingBuffer;
pthread_mutex_t Audio::mDspMutexLock;
int             Audio::mDspMutexCreated;
int             Audio::mDspThreadRunning;

int Audio::Create( )
{
    // common implementation found in FBA ports
    int bufferLength = ( SAMPLE_RATE * 100 + (DEFAULT_FPS >> 1) ) / DEFAULT_FPS;
    int result = Audio::SetBufferLength( bufferLength );

    if( result < 0 )
    {
        flushPrintf( "Audio::Create() - Failed to set ASP buffer length.\r\n" );
        return -1;
    }

	mDspHandle = open( SOUND_DEVICE, O_WRONLY );
	if( mDspHandle < 0 )
	{
        flushPrintf( "Audio::Create() Failed to open sound device!\r\n" );
        
        Destroy( );
		return -1;
	}
    
	// see http://manuals.opensound.com/developer/SNDCTL_DSP_SETFRAGMENT.html
    // the high 16 bits store the number of buffer fragments. The low 16 bits
    // is a selector that maps to a buffer size. There's no guarantee
    // the driver will respect it, but we try.
    int numFragments   = 10;
    int prefBufferSize = 11; //11 Maps to 2048
    int fragBufferHint = (numFragments << 16)  | prefBufferSize;
    
	result = ioctl(mDspHandle, SNDCTL_DSP_SETFRAGMENT, &fragBufferHint );
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDCTL_DSP_SETFRAGMENT failed. Errno: %d\r\n", errno );
        
        Destroy( );
		return -1;
    }

	int muteVal = 0;
	result = ioctl( mDspHandle, SOUND_MIXER_WRITE_MUTE, &muteVal );
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SOUND_MIXER_WRITE_MUTE failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    // Configure Speaker State
    // These setup audio to go out via HDMI vs speaker
    int sOutMode = SNDRV_OUTMODE_HDMI;
	result = ioctl(mDspHandle, SNDRV_OUTMODE, &sOutMode);
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDRV_SOUTMODE failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    int sh2DeviceState = SNDRV_SH2WDEVICE_HDMI;
	ioctl(mDspHandle, SNDRV_SH2WDEVICE, &sh2DeviceState);
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDRV_SH2WDEVICE failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    int hwInitVal = SNDRV_PLAY_HWINIT_HDMI;
	ioctl(mDspHandle, SNDRV_PLAY_HWINIT, &hwInitVal);
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDRV_PLAY_HWINIT failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    // This one turns off said speaker (not sure if its needed)
    int speakerOn = SNDRV_SPEAKER_OFF;
	ioctl(mDspHandle, SNDRV_SPEAKER, &speakerOn);
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDRV_SSPEAKER failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    // Configure playback rate
    // This doesnt appear to work and doesn't seem to matter, so i
    // assume the default matches what we need
    /*int bitsPerSample = SAMPLE_BITS_PER_SAMPLE;
    result = ioctl( mDspHandle, SNDCTL_DSP_SAMPLESIZE, &bitsPerSample );
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDCTL_DSP_SAMPLESIZE failed. Errno: %d\r\n", errno );
    }*/

    int channels = SAMPLE_NUM_CHANNELS;
	result = ioctl( mDspHandle, SNDCTL_DSP_CHANNELS, &channels );
    if ( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDCTL_DSP_CHANNELS: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    int sampleRate = SAMPLE_RATE;
	result = ioctl( mDspHandle, SNDCTL_DSP_SPEED, &sampleRate );
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDCTL_DSP_SPEED: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
        
    // Setup audio and volume threads
    pthread_mutex_init( &mDspMutexLock, NULL );
    mDspMutexCreated = 1;
    
    pthread_t audioT;
    result = pthread_create( &audioT, NULL, UpdateAudio_ThreadProc, NULL );
    if( result != 0 )
    {
        flushPrintf( "Audio::Create() pthread_create failed with error: %d\r\n", result );
        
        Destroy( );
        return -1;
    }
    
    pthread_detach( audioT );
    
    return 0;
}

void Audio::Destroy( )
{
    if( mDspHandle > 0 )
    {
        close( mDspHandle );
        mDspHandle = -1;
    }
    
    if( mDspMutexCreated )
    {
        pthread_mutex_lock( &mDspMutexLock );
        pthread_mutex_unlock( &mDspMutexLock );
        pthread_mutex_destroy( &mDspMutexLock );
        
        mDspMutexCreated = 0;
    }
    
    mDspThreadRunning    = 0;
}

void Audio::PlayBuffer( char *pBuffer, int bytes )
{
    pthread_mutex_lock( &mDspMutexLock );
    
    mDspRingBuffer.FillBuffer( pBuffer, bytes );
    
    pthread_mutex_unlock( &mDspMutexLock );
}

int Audio::SetBufferLength( int samplesPerFrame )
{
    mSamplesPerTick = samplesPerFrame;
    
    // make sure our read buffer can handle more than what we'd need for a frame
    // by doubling it
    int bufferLength = (samplesPerFrame * SAMPLE_BLOCK_ALIGN) * 2;
    if( bufferLength > MAX_AUDIO_BUFFER_BYTES )
    {
        flushPrintf( "Audio::SetBufferLength() - Needed buffer length more than supported. Need: %d Have: %d\r\n", 
                      bufferLength, 
                      MAX_AUDIO_BUFFER_BYTES );
        return -1;
    }
    
    return 0;
}

void *Audio::UpdateAudio_ThreadProc( void *pArg)
{
    mDspThreadRunning = 1;
    
    while( mDspThreadRunning )
    {
        pthread_mutex_lock( &mDspMutexLock );
        
        int bytesPerGameFrame = mSamplesPerTick * SAMPLE_BLOCK_ALIGN;
        if( mDspRingBuffer.GetBytesFilled( ) >= bytesPerGameFrame )
        {
            int bytesRead = mDspRingBuffer.ReadBuffer( mReadBuffer, bytesPerGameFrame );
            
            int bytesWritten = write( mDspHandle, mReadBuffer, bytesRead );
            
            pthread_mutex_unlock( &mDspMutexLock );
        }
        else
        {
            // if our buffer is empty, sleep until we're refilled
            pthread_mutex_unlock( &mDspMutexLock );
            usleep( 1000 );
        }
    }
    
    return NULL;
}
