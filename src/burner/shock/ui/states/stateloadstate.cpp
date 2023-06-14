
// See License.md for license

#include "shock/core/ostimer.h"
#include "shock/ui/states/stateloadstate.h"
#include "shock/input/shockinput.h"
#include "shock/shockgame.h"

void StateLoadState::Create( )
{
    StateGameStateBase::Create( );
}

void StateLoadState::Destroy( )
{
    StateGameStateBase::Destroy( );
}

void StateLoadState::EnterState( UIState oldState )
{
    StateGameStateBase::EnterState( oldState );
}

void StateLoadState::ExitState( UIState newState )
{
    StateGameStateBase::ExitState( newState );
}

void StateLoadState_OnComplete( int result, void *pInstance )
{
    ( (StateLoadState *)pInstance )->OnSaveLoadComplete( result );
}

UIState StateLoadState::Update( )
{
    // dont allow input while showing the processing ui
    if ( mSaveLoadThreadState == SaveLoadThreadState_None
        && mProcessingUITimerMS < gGlobalTimer.GetElapsedTimeMicroseconds( ) )
    {
        if ( ShockInput::GetInput( P1_Button_1 )->WasReleased( ) )
        {
            if ( mStateExists[ mMenuSelection ] )
            {
                ShowProcessingUI( );
                ShockGame::LoadGameState( mMenuSelection, StateLoadState_OnComplete, this );
            }
        }
    }

    // watch for completion
    if ( mSaveLoadThreadState == SaveLoadThreadState_Complete )
    {
        if ( mSaveLoadResult == 1 )
        {
            snprintf( mResultStr, sizeof( mResultStr ), "State for slot %d loaded. Return to game to continue.", mMenuSelection + 1 );
        }
        else
        {
            snprintf( mResultStr, sizeof( mResultStr ), "Error loading state for slot: %d. Check USB Drive and scan for errors.", mMenuSelection + 1 );
        }

        mSaveLoadThreadState = SaveLoadThreadState_None;
        mSaveLoadResult = 0;
    }

    StateGameStateBase::DrawMenu( "LOAD STATE", "LOADING..." );

    return StateGameStateBase::Update( );
}
