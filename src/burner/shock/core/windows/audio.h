
// See License.md for license

#ifndef AUDIO_H_
#define AUDIO_H_

class Audio
{
public:
    static int  Create( );
    static void Destroy( );
    static void PlayBuffer( char *pBuffer, int bytes );
    static int  NeedBufferRefill( );
};

#endif
