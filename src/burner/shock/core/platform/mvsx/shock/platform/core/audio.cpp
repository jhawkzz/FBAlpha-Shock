
// See License.md for license

#include "shock/core/audio.h"
#include "shock/util/util.h"

int             AudioCore::mSamplesPerTick;
char            AudioCore::mReadBuffer[ MAX_AUDIO_BUFFER_BYTES ];
int             AudioCore::mDspHandle;
RingBuffer      AudioCore::mDspRingBuffer;
pthread_mutex_t AudioCore::mDspMutexLock;
int             AudioCore::mDspMutexCreated;
int             AudioCore::mDspThreadRunning;
int             AudioCore::mVolume;
int             AudioCore::mVolumeThreadRunning;
UINT8           AudioCore::mVolumeLookup[ SPEAKER_MAX_VALUE + 1 ];

int Audio::Create( )
{
    // common implementation found in FBA ports
    int bufferLength = ( SAMPLE_RATE * 100 + (DEFAULT_FPS >> 1) ) / DEFAULT_FPS;
    int result = Audio::SetBufferLength( bufferLength );

    if( result < 0 )
    {
        flushPrintf( "ShockAudio::Create() - Failed to set MVSX buffer length.\r\n" );
        return -1;
    }

	mDspHandle = open( SOUND_DEVICE, O_WRONLY );
	if( mDspHandle < 0 )
	{
        flushPrintf( "Audio::Create() Failed to open sound device!\r\n" );
        
        Destroy( );
		return -1;
	}
    
    CreateVolumeLookup( );

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
    // These setup audio to go out via speaker vs HDMI
    int sOutMode = SNDRV_OUTMODE_SPEAKER;
	result = ioctl(mDspHandle, SNDRV_OUTMODE, &sOutMode);
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDRV_SOUTMODE failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    int sh2DeviceState = SNDRV_SH2WDEVICE_SPEAKER;
	ioctl(mDspHandle, SNDRV_SH2WDEVICE, &sh2DeviceState);
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDRV_SH2WDEVICE failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    int hwInitVal = SNDRV_PLAY_HWINIT_SPEAKER;
	ioctl(mDspHandle, SNDRV_PLAY_HWINIT, &hwInitVal);
    if( result < 0 )
    {
        flushPrintf( "Audio::Create() SNDRV_PLAY_HWINIT failed. Errno: %d\r\n", errno );
        
        Destroy( );
        return -1;
    }
    
    // This one turns ON said speaker
    int speakerOn = SNDRV_SPEAKER_ON;
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
    
    pthread_t volumeT;
    result = pthread_create( &volumeT, NULL, UpdateVolume_ThreadProc, NULL );
    if( result != 0 )
    {
        flushPrintf( "Audio::Create() pthread_create failed with error: %d\r\n", result );
        
        Destroy( );
        return -1;
    }
    
    pthread_detach( volumeT );
    
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
    mVolumeThreadRunning = 0;
}

void Audio::PlayBuffer( char *pBuffer, int bytes )
{
    pthread_mutex_lock( &mDspMutexLock );
    
    mDspRingBuffer.FillBuffer( pBuffer, bytes );
    
    pthread_mutex_unlock( &mDspMutexLock );
}

int AudioCore::SetBufferLength( int samplesPerFrame )
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

void *AudioCore::UpdateAudio_ThreadProc( void *pArg)
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

void *AudioCore::UpdateVolume_ThreadProc( void *pArg )
{
    mVolumeThreadRunning = 1;
    
    while( mVolumeThreadRunning )
    {   
        // we have to open / close each loop iteration so we 
        // dont hold a lock and prevent the knob from updating.
        int volumeHandle = open( VOLUME_DEVICE, O_RDONLY );
        if( volumeHandle > -1 )
        {
            // poll the volume device 
            char buffer[ 32 ] = { 0 };
            int result = read( volumeHandle, buffer, sizeof( buffer ) );
            if( result > 0 )
            {
                int rawVolumeVal = atoi( buffer );
                
                // Volume is held in reverse, so flip, then convert to %, then scale to speaker vals
                float inverseVolume = (float) VOLUME_KNOB_MAX_VALUE - rawVolumeVal;
                float volPerc       = inverseVolume / VOLUME_KNOB_MAX_VALUE;
                int newVolume       = (int) (volPerc * SPEAKER_MAX_VALUE);
                
                if ( newVolume != mVolume )
                {
                    newVolume = min( newVolume, SPEAKER_MAX_VALUE );
                    mVolume = mVolumeLookup[ newVolume ];
                    
                    if ( ioctl( mDspHandle, SOUND_MIXER_WRITE_VOLUME, &mVolume ) == -1) 
                    {
                        flushPrintf("Audio::UpdateVolume_ThreadProc() SOUND_MIXER_WRITE_VOLUME failed!\r\n");
                    }
                }
            }
            
            close( volumeHandle );
        }
        else
        {
            flushPrintf( "Audio::UpdateVolume_ThreadProc() Failed to open volume handle!\r\n" );
        }
        
        usleep( 200 * MILLI_TO_MICROSECONDS );
    }
}

void AudioCore::CreateVolumeLookup( )
{
    // the first half of the dial is just too low
    // so scale things up
    mVolumeLookup[ 0 ] = 0;
    mVolumeLookup[ 1 ] = 10;
    mVolumeLookup[ 2 ] = 14;
    mVolumeLookup[ 3 ] = 16;
    mVolumeLookup[ 4 ] = 18;
    mVolumeLookup[ 5 ] = 20;
    mVolumeLookup[ 6 ] = 22;
    mVolumeLookup[ 7 ] = 23;
    mVolumeLookup[ 8 ] = 23;
    mVolumeLookup[ 9 ] = 24;
    mVolumeLookup[ 10 ] = 24;
    mVolumeLookup[ 11 ] = 25;
    mVolumeLookup[ 12 ] = 25;
    mVolumeLookup[ 13 ] = 26;
    mVolumeLookup[ 14 ] = 26;
    mVolumeLookup[ 15 ] = 27;
    mVolumeLookup[ 16 ] = 27;
    mVolumeLookup[ 17 ] = 28;
    mVolumeLookup[ 18 ] = 28;
    mVolumeLookup[ 19 ] = 29;
    mVolumeLookup[ 20 ] = 29;
    mVolumeLookup[ 21 ] = 30;
    mVolumeLookup[ 22 ] = 30;
    mVolumeLookup[ 23 ] = 31;
    mVolumeLookup[ 24 ] = 31;
    mVolumeLookup[ 25 ] = 32;
    mVolumeLookup[ 26 ] = 32;
    mVolumeLookup[ 27 ] = 33;
    mVolumeLookup[ 28 ] = 33;
    mVolumeLookup[ 29 ] = 34;
    mVolumeLookup[ 30 ] = 34;
    mVolumeLookup[ 31 ] = 35;
    mVolumeLookup[ 32 ] = 35;
    mVolumeLookup[ 33 ] = 36;
    mVolumeLookup[ 34 ] = 36;
    mVolumeLookup[ 35 ] = 37;
    mVolumeLookup[ 36 ] = 37;
    mVolumeLookup[ 37 ] = 37;
    mVolumeLookup[ 38 ] = 38;
    mVolumeLookup[ 39 ] = 38;
    mVolumeLookup[ 40 ] = 40;
}
