
// See License.md for license

#include "shock/core/ostimer.h"
#include "shock/input/shockinput.h"

InputControl ShockInput::mInputState[ ShockButton_Count ];

// Input Control
void InputControl::Create( )
{    
    mPrevState       = InputState_Released;
    mState           = InputState_Released;
    mTimeHeldMS      = 0;
    mTimeHeldStartMS = 0;   
    mTimeHeldEndMS   = 0;
}

int InputControl::WasPressed( )
{
    return mPrevState == 0 && mState == InputState_Pressed;
}

int InputControl::WasReleased( )
{
    return mPrevState == InputState_Pressed && mState == 0;
}

UINT64 InputControl::GetTimeHeldMS( )
{
    return mTimeHeldMS;
}

UINT64 InputControl::GetTimePressedMS( )
{
    return mTimeHeldStartMS;
}

UINT64 InputControl::GetTimeReleasedMS( )
{
    return mTimeHeldEndMS;
}

InputState InputControl::GetState( )
{
    return mState;
}

void InputControl::Update( int newState )
{
    mPrevState = mState;
    mState = (InputState)newState;
    
    if( WasPressed( ) )
    {
        mTimeHeldStartMS = gGlobalTimer.GetElapsedTimeMicroseconds();
    }
    else if ( WasReleased( ) )
    {
        mTimeHeldEndMS = gGlobalTimer.GetElapsedTimeMicroseconds();
    }
    
    // if its' being held right now, track how long.
    if( mState == InputState_Pressed )
    {
        mTimeHeldMS = gGlobalTimer.GetElapsedTimeMicroseconds() - mTimeHeldStartMS;
    }
}
// End Input Control

int ShockInput::Create( )
{
    for( int i = 0; i < ShockButton_Count; i++ )
    {
        mInputState[ i ].Create( );
    }
    
    return Input::Create( );
}

void ShockInput::Destroy( )
{
}

void ShockInput::Update( )
{
    for( int i = 0; i < ShockButton_Count; i++ )
    {
        int inputVal = Input::GetValueForButton( (ShockButton) i );
        mInputState[ i ].Update( inputVal );
    }
}

InputControl *ShockInput::GetInput( ShockButton button )
{
    return &mInputState[ button ];
}
