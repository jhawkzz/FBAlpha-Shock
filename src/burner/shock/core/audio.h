
// See License.md for license

#ifndef AUDIO_H_
#define AUDIO_H_

#include "shock/platform/core/audio.h"

class Audio : public AudioCore
{
public:
    static int  Create( );
    static void Destroy( );
    static void PlayBuffer( char *pBuffer, int bytes );
    static int  NeedBufferRefill( );
};

#endif
