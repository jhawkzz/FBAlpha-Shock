
// See License.md for license

#include "shock/shock.h"
#include "shock/shockmain.h"
#include "shock/util/util.h"

/* NOTE
 * If running on x86 Lubuntu, this renders directly to the frame buffer. 
 * That means you'll need to enter a terminal mode by pressing Ctrl + Alt + F1
 * and launching it from there. Press Ctrl + Alt + F7 to return.
*/

int main( int argc, char **argv )
{
    flushPrintf( "-=-=-=-=FBA 100 Mega Shock Edition=-=-=-=-\r\n" );
    
    // ensure args are right
    if( argc != 2 )
    {
        printf( "Usage: %s romset.zip\r\n", argv[ 0 ] );
        return 0;
    }
    
    ShockMain::Run( argv[ 1 ] );
    
    return 0;
}
