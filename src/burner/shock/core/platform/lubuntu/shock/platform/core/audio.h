
// See License.md for license

#ifndef AUDIO_CORE_H_
#define AUDIO_CORE_H_

#define PCM_DEVICE                "default"
#define MAX_PCM_BUFFER_SIZE_BYTES (1024 * 1024)

class AudioCore
{
protected:
    static void *UpdateAudio_ThreadProc( void *pArg );

protected:
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
