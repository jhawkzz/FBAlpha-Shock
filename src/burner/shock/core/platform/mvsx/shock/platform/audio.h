
// See License.md for license

#ifndef AUDIO_CORE_H_
#define AUDIO_CORE_H_

#define SPEAKER_MAX_VALUE         (40) // Max value to scale audio by. Any louder could damage the speakers
#define VOLUME_KNOB_MAX_VALUE     (82) // Volume knob is 82 - 0 reversed (all the way left is 82, all the way right is 0)

// The path to the sound device and volume knob on the MVSX Linux OS.
// We read the value at VOLUME_DEVICE and multiply the sound we write to SOUND_DEVICE to effect volume.
#define SOUND_DEVICE	             "/dev/snd/actsnd"
#define VOLUME_DEVICE             "/sys/class/volume/value"

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

#define MAKE_STEREO_VOLUME(a)     ((a) << 8 | (a))

class AudioCore
{
public:
    static int SetBufferLength( int samplesPerFrame );

protected:
    static int GetVolume( );
    static void *UpdateAudio_ThreadProc( void *pArg );
    static void *UpdateVolume_ThreadProc( void *pArg );
    static void  CreateVolumeLookup( );

    static int             mSamplesPerTick;
    static char            mReadBuffer[ MAX_AUDIO_BUFFER_BYTES ];
    
    static int             mDspHandle;
    static RingBuffer      mDspRingBuffer;
    static pthread_mutex_t mDspMutexLock;
    static int             mDspMutexCreated;
    static int             mDspThreadRunning;
    
    static int             mVolume; //No need for mutex, we only read across threads, no writing.
    static int             mVolumeThreadRunning;
    static UINT8           mVolumeLookup[ SPEAKER_MAX_VALUE + 1 ];
};

#endif
