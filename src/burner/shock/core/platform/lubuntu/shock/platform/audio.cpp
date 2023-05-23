
// See License.md for license

#include "shock/includes.h"
#include "shock/core/audio.h"

unsigned long   AudioImpl::mSampleFramesPerTick;
snd_pcm_t      *AudioImpl::mPcmHandle;
int             AudioImpl::mPcmBufferSize;
char            AudioImpl::mPcmFeedBuffer[ MAX_PCM_BUFFER_SIZE_BYTES ];

RingBuffer      AudioImpl::mDspRingBuffer;
pthread_mutex_t AudioImpl::mDspMutexLock;
int             AudioImpl::mDspMutexCreated;
int             AudioImpl::mDspThreadRunning;

int Audio::Create( )
{
	// Open the PCM device in playback mode
	int result = snd_pcm_open( &mPcmHandle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0 );
    if( result < 0 )
    {
		flushPrintf( "Audio::Create() - ERROR Can't open %s PCM device. %s\n", PCM_DEVICE, snd_strerror( result ) );
        return -1;
    }
    
	// Allocate parameters object and fill it with default values
    snd_pcm_hw_params_t *pParams;
	snd_pcm_hw_params_alloca(&pParams);
	snd_pcm_hw_params_any( mPcmHandle, pParams);

	// set our playback values (note interleaved so it knows LR/LR/LR sample frames
	result = snd_pcm_hw_params_set_access( mPcmHandle, pParams, SND_PCM_ACCESS_RW_INTERLEAVED );
    if ( result < 0 )
    {
		flushPrintf( "Audio::Create() - ERROR Can't set interleaved mode. %s\n", snd_strerror( result ) );
        return -1;
    }
    
    if( SAMPLE_BITS_PER_SAMPLE != 16 )
    {
        flushPrintf( "Audio::Create() - ERROR We only suppotr 16 bit audio.\n" );
        return -1;
    }
	
    result = snd_pcm_hw_params_set_format( mPcmHandle, pParams, SND_PCM_FORMAT_S16_LE );
    if( result < 0 )
    {
		flushPrintf( "Audio::Create() - ERROR Can't set format. %s\n", snd_strerror( result ) );
        return -1;
    }

	result = snd_pcm_hw_params_set_channels( mPcmHandle, pParams, SAMPLE_NUM_CHANNELS );
    if( result < 0 )
    {
		flushPrintf( "Audio::Create() - ERROR Can't set channels number. %s\n", result );
        return -1;
    }

    UINT32 rate = SAMPLE_RATE;
	result = snd_pcm_hw_params_set_rate_near( mPcmHandle, pParams, &rate, 0 );
    if( result < 0 )
    {
		flushPrintf("Audio::Create() - ERROR Can't set playback rate. %s\n", snd_strerror( result ) );
        return -1;
    }

	// write out all params
	result = snd_pcm_hw_params( mPcmHandle, pParams );
    if( result < 0 )
    {
		flushPrintf( "Audio::Create() - ERROR Can't set harware parameters. %s\n", snd_strerror( result ) );
        return -1;
    }

	//flushPrintf("Audio::Create() - PCM Device Playback Name: %s\n", snd_pcm_name( mPcmHandle ) );

	// get the number of sample frames per tick that it wants 
    // NOTE - It refers to these frames as "periods"
	snd_pcm_hw_params_get_period_size( pParams, &mSampleFramesPerTick, 0 );
    //flushPrintf( "Audio::Create() - Periods (Sample Frames) Per Tick: %lu\r\n", mSampleFramesPerTick );
    
    // buffer size should be enough for 2 frame / period's worth of audio
	mPcmBufferSize = mSampleFramesPerTick * SAMPLE_NUM_CHANNELS * 2;
    if( mPcmBufferSize > MAX_PCM_BUFFER_SIZE_BYTES )
    {
        flushPrintf( "Audio::Create() - PCM Buffer Size Required too large. Needed: %d, Supports: %d\r\n", 
                     mPcmBufferSize, 
                     MAX_PCM_BUFFER_SIZE_BYTES );
        return -1;
    }

    // to ME, the audio feels off by a few frames,
    // and this period time hints at the latency you need to factor in.
    // However, its consistent (doesnt fall behind), only on Lubuntu, and i need to move on.
    /*UINT32 tmp;
	snd_pcm_hw_params_get_period_time( pParams, &tmp, NULL);
    printf("period time: %d\r\n", tmp );*/
    
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
    if ( mPcmHandle != NULL )
    {
        snd_pcm_drain( mPcmHandle );
        snd_pcm_close( mPcmHandle );
        
        mPcmHandle = NULL;
    }
    
    if( mDspMutexCreated )
    {
        pthread_mutex_lock( &mDspMutexLock );
        pthread_mutex_unlock( &mDspMutexLock );
        pthread_mutex_destroy( &mDspMutexLock );
        
        mDspMutexCreated = 0;
    }
    
    mDspThreadRunning = 0;
}

void Audio::PlayBuffer( char *pBuffer, int bytes )
{
    pthread_mutex_lock( &mDspMutexLock );
    
    //flushPrintf("storing %d bytes into buffer\r\n", bytes );
    mDspRingBuffer.FillBuffer( pBuffer, bytes );
    
    pthread_mutex_unlock( &mDspMutexLock );
}

int Audio::NeedBufferRefill( )
{
    pthread_mutex_lock( &mDspMutexLock );
    
    int bytes = mDspRingBuffer.GetBytesFilled( );
    
    pthread_mutex_unlock( &mDspMutexLock );
    
    // if the buffer is empty, we do need more data
    if( bytes == 0 )
    {
        return 1;
    }
    
    return 0;
}

void *AudioImpl::UpdateAudio_ThreadProc( void *pArg)
{
    mDspThreadRunning = 1;
    
    while( mDspThreadRunning )
    {
        pthread_mutex_lock( &mDspMutexLock );
        
        if( mDspRingBuffer.GetBytesFilled( ) >= mPcmBufferSize )
        {
            int bytesRead = mDspRingBuffer.ReadBuffer( mPcmFeedBuffer, mPcmBufferSize );
            
            //flushPrintf("Writing %d bytes (%d frames) into buffer\r\n", bytesRead, mSampleFramesPerTick );
            int result = snd_pcm_writei( mPcmHandle, mPcmFeedBuffer, mSampleFramesPerTick );
            
            // sometimes ALSA reports an underrun, and we just need to reinit 
            // and try again. (Without doing this, audio skips badly)
            if( result == -EPIPE )
            {
                snd_pcm_prepare( mPcmHandle );
                snd_pcm_writei( mPcmHandle, mPcmFeedBuffer, mSampleFramesPerTick );
                // who cares about the result - if it didnt work, we will just drop the frame sample (alsa period)
            } 
            else if ( result  < 0 ) 
            {
                printf("Audio::UpdateAudio_ThreadProc() - ERROR. Can't write to PCM device. %s\n", snd_strerror( result ) );
            }
            
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
