
// See License.md for license

#ifndef SHOCKINPUT_H_
#define SHOCKINPUT_H_

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
    long       GetTimeHeldMS( );
    long       GetTimePressedMS( );
    long       GetTimeReleasedMS( );
    InputState GetState( );
    void       Update( int newState );
    
private:
    InputState mPrevState;
    InputState mState;
    long       mTimeHeldMS;
    
    // store this seperate from timeHeld. Otherwise
    // when holding the button, timeHeld will be
    // the timestamp of when the button was pressed, which
    // isn't accurate at all.
    long       mTimeHeldStartMS;
    long       mTimeHeldEndMS;
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
