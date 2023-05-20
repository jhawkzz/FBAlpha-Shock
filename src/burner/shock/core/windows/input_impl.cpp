
// See License.md for license

#include "../input.h"
#include "../../includes.h"

namespace
{
    UINT8 KeyMappings[Button_Count]
    {
        /*P1_Start     = */0x31, // Keyboard 1
        /*OptionsBack  = */0x35, // Keyboard 5
        /*SelectGame   = */0x36, // Keyboard 6
        /*P2_Start     = */0x33, // Keyboard 2

        /*P2_Yellow    = */0x59, // Keyboard Y
        /*P2_Green     = */0x55, // Keyboard U
        /*P2_Blue      = */0x49, // Keyboard I

        /*P1_Yellow    = */0x41, // Keyboard A
        /*P1_Green     = */0x53, // Keyboard S
        /*P1_Blue      = */0x44, // Keyboard D

        /*P2_Red       = */0x48, // Keyboard H
        /*P2_MidBot    = */0x4a, // Keyboard J
        /*P2_RightBot  = */0x4b, // Keyboard K

        /*P1_Red       = */0x5a, // Keyboard Z
        /*P1_MidBot    = */0x58, // Keyboard X
        /*P1_RightBot  = */0x43, // Keyboard C

        /*P1_Joy_Up    = */VK_UP,    // Keyboard Up Arrow
        /*P2_Joy_Up    = */VK_HOME,  // Keyboard Home
        /*P1_Joy_Right = */VK_RIGHT, // Keyboard Right Arrow
        /*P2_Joy_Down  = */VK_END,   // Keyboard End
        /*P1_Joy_Down  = */VK_DOWN,  // Keyboard Down Arrow
        /*P1_Joy_Left  = */VK_LEFT,  // Keyboard Left Arrow
        /*P2_Joy_Right = */VK_NEXT,  // Keyboard PgDn
        /*P2_Joy_Left  = */VK_DELETE,// Keyboard Delete
    };
};

int Input::Create( )
{
    return 0;
}

void Input::Destroy( )
{
}

int Input::GetValueForInput( InputCodeToButtonMapping input )
{
    SHORT s = GetAsyncKeyState(KeyMappings[input]);
    return s & 0x8000 ? 1 : 0;
}
