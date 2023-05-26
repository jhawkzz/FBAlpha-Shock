
// See License.md for license

#ifndef AUDIO_CORE_H_
#define AUDIO_CORE_H_

// The path to the sound device on the ASP Linux OS.
#define SOUND_DEVICE	          "/dev/snd/actsnd"

#define SNDRV_OUTMODE             (0xFFFF0000)
#define SNDRV_OUTMODE_SPEAKER     (0)
#define SNDRV_OUTMODE_HDMI        (5)

#define SNDRV_SH2WDEVICE	      (0xFFFF0003)
#define SNDRV_SH2WDEVICE_SPEAKER  (0)
#define SNDRV_SH2WDEVICE_HDMI     (5)

#define SNDRV_PLAY_HWINIT         (0xFFFF0005)
#define SNDRV_PLAY_HWINIT_SPEAKER (0)
#define SNDRV_PLAY_HWINIT_HDMI    (5)

#define SNDRV_SPEAKER		      (0xFFFF0001)
#define SNDRV_SPEAKER_OFF         (0)
#define SNDRV_SPEAKER_ON          (1) //Any non-zero value works

class AudioCore
{
public:
    static int  SetBufferLength( int samplesPerFrame );

protected:
    static void *UpdateAudio_ThreadProc( void *pArg );

    static int             mSamplesPerTick;
    static char            mReadBuffer[ MAX_AUDIO_BUFFER_BYTES ];
    
    static int             mDspHandle;
    static RingBuffer      mDspRingBuffer;
    static pthread_mutex_t mDspMutexLock;
    static int             mDspMutexCreated;
    static int             mDspThreadRunning;
};

#endif
