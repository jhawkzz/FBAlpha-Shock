
// See License.md for license

#include "includes.h"

int ShockAudio::Create( )
{   
#ifdef X86
    return X86Audio::Create( );
#elif MVSX
    // common implementation found in FBA ports
    int bufferLength = ( SAMPLE_RATE * 100 + (DEFAULT_FPS >> 1) ) / DEFAULT_FPS;
    int result = MVSXAudio::SetBufferLength( bufferLength );
    if( result < 0 )
    {
        flushPrintf( "ShockAudio::Create() - Failed to set MVSX buffer length.\r\n" );
        return -1;
    }
    return MVSXAudio::Create( );
#endif
}

void ShockAudio::Destroy( )
{
}

void ShockAudio::PlayBuffer( char *pBuffer, int bufferSize )
{
#ifdef X86
    X86Audio::PlayBuffer( pBuffer, bufferSize );
#elif MVSX
    MVSXAudio::PlayBuffer( pBuffer, bufferSize );
#endif    
}
