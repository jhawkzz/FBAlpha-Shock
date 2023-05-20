
// See License.md for license

#include "includes.h"
#include "core/audio.h"

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
