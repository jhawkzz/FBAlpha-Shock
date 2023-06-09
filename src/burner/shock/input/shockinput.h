
// See License.md for license

#ifndef SHOCKINPUT_H_
#define SHOCKINPUT_H_

#include "shock/shock.h"
#include "shock/core/input.h"

enum InputState
{
    InputState_Released = 0,
    InputState_Pressed  = 1
};

// NOTE: All times are in microseconds
class InputControl
{
public:
    void       Create( );
    int        WasPressed( );
    int        WasReleased( );
    UINT64     GetTimeHeldMS( );
    UINT64     GetTimePressedMS( );
    UINT64     GetTimeReleasedMS( );
    InputState GetState( );
    void       Update( int newState );
    
private:
    InputState mPrevState;
    InputState mState;
    UINT64     mTimeHeldMS;
    
    // store this seperate from timeHeld. Otherwise
    // when holding the button, timeHeld will be
    // the timestamp of when the button was pressed, which
    // isn't accurate at all.
    UINT64     mTimeHeldStartMS;
    UINT64     mTimeHeldEndMS;
};

class ShockInput
{
public:
    static int  Create( );
    static void Destroy( );
    
    static void Update( );
    
    static InputControl *GetInput( ShockButton button );
    
private:
    static InputControl mInputState[ ShockButton_Count ];
};

#endif
