
// See License.md for license

#ifndef BUTTON_H_
#define BUTTON_H_

enum ShockButton
{
    P1_Joy_Up = 0,
    P1_Joy_Left = 1,
    P1_Joy_Right = 2,
    P1_Joy_Down = 3,

    P1_InsertCoin = 4,
    P1_Start = 5,

    P1_Button_1  = 6,
    P1_Button_2  = 7,
    P1_Button_3  = 8,
    P1_Button_4  = 9,
    P1_Button_5  = 10,
    P1_Button_6  = 11,
    P1_Button_7  = 12,
    P1_Button_8  = 13,

    P2_Joy_Up = 14,
    P2_Joy_Left = 15,
    P2_Joy_Right = 16,
    P2_Joy_Down = 17,

    P2_InsertCoin = 18,
    P2_Start = 19,

    P2_Button_1 = 20,
    P2_Button_2 = 21,
    P2_Button_3 = 22,
    P2_Button_4 = 23,
    P2_Button_5 = 24,
    P2_Button_6 = 25,
    // These buttons aren't currently mapped anywhere
    // on MVSX (only 6 buttons each for 2 players) or ASP (only 8 buttons for one player)
    P2_Button_7 = 26, 
    P2_Button_8 = 27,

    OptionsMenu = 28, //On MVSX this will be bound to P1_InsertCoin, on ASP, the Options button

    ShockButton_Count = 29
};

#endif
