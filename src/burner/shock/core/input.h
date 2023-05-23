
// See License.md for license

#ifndef INPUT_H_
#define INPUT_H_

#include "shock/platform/input.h"

class Input : public InputImpl
{
public:
    static int  Create( );
    static void Destroy( );
    static int  GetValueForInput( InputCodeToButtonMapping input );
};

#endif
