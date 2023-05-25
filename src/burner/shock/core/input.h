
// See License.md for license

#ifndef INPUT_H_
#define INPUT_H_

#include "shock/platform/input.h"

enum ShockButton
{
    P1_Joy_Up = 0,
    P1_Joy_Left = 1,
    P1_Joy_Right = 2,
    P1_Joy_Down = 3,

    P1_InsertCoin = 4,
    P1_Start = 5,

    P1_Button_1 = 6,
    P1_Button_2 = 7,
    P1_Button_3 = 8,
    P1_Button_4 = 9,
    P1_Button_5 = 10,
    P1_Button_6 = 11,

    P2_Joy_Up = 12,
    P2_Joy_Left = 13,
    P2_Joy_Right = 14,
    P2_Joy_Down = 15,

    P2_InsertCoin = 16,
    P2_Start = 17,

    P2_Button_1 = 18,
    P2_Button_2 = 19,
    P2_Button_3 = 20,
    P2_Button_4 = 21,
    P2_Button_5 = 22,
    P2_Button_6 = 23,

    ShockButton_Count = 24
};

class Input : public InputCore
{
public:
    static int  Create( );
    static void Destroy( );
    int static GetValueForButton( ShockButton shockButton );
};

#endif
