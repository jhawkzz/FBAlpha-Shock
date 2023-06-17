
// See License.md for license

#include "shock/core/audio.h"
#include "shock/shockaudio.h"
#include "shock/shockprofilers.h"

int ShockAudio::Create( )
{   
    return Audio::Create( );
}

void ShockAudio::Destroy( )
{
    return Audio::Destroy();
}

void ShockAudio::PlayBuffer( char *pBuffer, int bufferSize )
{
    Audio::PlayBuffer( pBuffer, bufferSize );
}
