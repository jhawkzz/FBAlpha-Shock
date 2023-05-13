
// See License.md for license

#ifndef INPUT_H_
#define INPUT_H_

enum InputCodeToButtonMapping
{
    P1_Start,
    OptionsBack,
    SelectGame,
    P2_Start,

    P2_Yellow,
    P2_Green,
    P2_Blue,

    P1_Yellow,
    P1_Green,
    P1_Blue,

    P2_Red,
    P2_MidBot,
    P2_RightBot,

    P1_Red,
    P1_MidBot,
    P1_RightBot,

    P1_Joy_Up,
    P2_Joy_Up ,
    P1_Joy_Right,
    P2_Joy_Down,
    P1_Joy_Down,
    P1_Joy_Left,
    P2_Joy_Right,
    P2_Joy_Left,

    Button_Count = 24
};

struct ButtonState
{
    int             mutexCreated;
    HANDLE          mutexLock;
    int             value; // This is the action (0 is released, 1 is down)
};

class Input
{
public:
    static int  Create( );
    static void Destroy( );
    static int  GetValueForInput( InputCodeToButtonMapping input );
};

#endif
