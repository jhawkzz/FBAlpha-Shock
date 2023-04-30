
// See License.md for license

#ifndef SHOCKAUDIO_H_
#define SHOCKAUDIO_H_

class ShockAudio
{
public:
    static int  Create( );
    static void Destroy( );
    
    static void PlayBuffer( char *pBuffer, int bufferSize );
};

#endif
