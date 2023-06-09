
// See License.md for license

#include "shock/core/audio.h"
#include "shock/shockaudio.h"
#include "shock/shockprofiler.h"

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
    SHOCK_PROFILE;
    Audio::PlayBuffer( pBuffer, bufferSize );
}
