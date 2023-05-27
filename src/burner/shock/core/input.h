
// See License.md for license

#ifndef INPUT_H_
#define INPUT_H_

#include "shock/core/button.h"
#include "shock/platform/core/input.h"

class Input : public InputCore
{
public:
    static int  Create( );
    static void Destroy( );
    static int GetValueForButton( ShockButton shockButton );
};

#endif
