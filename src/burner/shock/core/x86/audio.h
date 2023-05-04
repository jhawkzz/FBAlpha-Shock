
// See License.md for license

#ifndef AUDIO_H_
#define AUDIO_H_

#define PCM_DEVICE                "default"
#define MAX_PCM_BUFFER_SIZE_BYTES (1024 * 1024)

class Audio
{
public:
    static int  Create( );
    static void Destroy( );
    static void PlayBuffer( char *pBuffer, int bytes );
    static int  NeedBufferRefill( );
    
private:
    static void *UpdateAudio_ThreadProc( void *pArg );

private:
    static unsigned long   mSampleFramesPerTick;
    static snd_pcm_t      *mPcmHandle;
    static int             mPcmBufferSize;
    static char            mPcmFeedBuffer[ MAX_PCM_BUFFER_SIZE_BYTES ];
    
    static RingBuffer      mDspRingBuffer;
    static pthread_mutex_t mDspMutexLock;
    static int             mDspMutexCreated;
    static int             mDspThreadRunning;
};

#endif
