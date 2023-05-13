
// See License.md for license

#include "shock/includes.h"

int main( int argc, char **argv )
{
    flushPrintf( "-=-=-=-=FBA 100 Mega Shock Edition=-=-=-=-\r\n" );
    
    // ensure args are right
    if( argc != 2 )
    {
        printf( "Usage: %s romset.zip\r\n", argv[ 0 ] );
        return 0;
    }
    
    return ShockMain::Run( argv[ 1 ] );
}
