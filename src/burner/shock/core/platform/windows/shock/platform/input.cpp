
// See License.md for license

#include "shock/core/input.h"
#include "shock/includes.h"

namespace
{
    UINT8 KeyMappings[ShockButton_Count]
    {
        /*P1_Joy_Up     = */VK_UP,    // Keyboard Up Arrow
        /*P1_Joy_Left   = */VK_LEFT,  // Keyboard Left Arrow
        /*P1_Joy_Right  = */VK_RIGHT, // Keyboard Right Arrow
        /*P1_Joy_Down   = */VK_DOWN,  // Keyboard Down Arrow

        /*P1_InsertCoin = */0x35, // Keyboard 5
        /*P1_Start      = */0x31, // Keyboard 1
        
        /*P1_Button_1   = */0x5a, // Keyboard Z
        /*P1_Button_2   = */0x41, // Keyboard A
        /*P1_Button_3   = */0x53, // Keyboard S
        /*P1_Button_4   = */0x44, // Keyboard D
        /*P1_Button_5   = */0x58, // Keyboard X
        /*P1_Button_6   = */0x43, // Keyboard C

        /*P2_Joy_Up     = */VK_HOME,  // Keyboard Home
        /*P2_Joy_Left   = */VK_DELETE,// Keyboard Delete
        /*P2_Joy_Right  = */VK_NEXT,  // Keyboard PgDn
        /*P2_Joy_Down   = */VK_END,   // Keyboard End

        /*P2_InsertCoin = */0x36, // Keyboard 6
        /*P2_Start      = */0x33, // Keyboard 2

        
        /*P2_Button_1   = */0x48, // Keyboard H
        /*P2_Button_2   = */0x59, // Keyboard Y
        /*P2_Button_3   = */0x55, // Keyboard U
        /*P2_Button_4   = */0x49, // Keyboard I
        /*P2_Button_5   = */0x4a, // Keyboard J
        /*P2_Button_6   = */0x4b, // Keyboard K
    };
};

int Input::Create( )
{
    return 0;
}

void Input::Destroy( )
{
}

int Input::GetValueForButton( ShockButton shockButton )
{
    SHORT s = GetAsyncKeyState(KeyMappings[shockButton]);
    return s & 0x8000 ? 1 : 0;
}
