
// See License.md for license

#ifndef AUDIO_H_
#define AUDIO_H_

#include "audio_impl.h"

class Audio : public AudioImpl
{
public:
    static int  Create( );
    static void Destroy( );
    static void PlayBuffer( char *pBuffer, int bytes );
    static int  NeedBufferRefill( );
};

#endif
