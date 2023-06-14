
// See License.md for license

#include "shock/core/ostimer.h"
#include "shock/shockgame.h"
#include "shock/input/shockinput.h"
#include "shock/ui/render/uirenderer.h"
#include "shock/ui/states/statesavestate.h"

void StateSaveState::Create( )
{
    StateGameStateBase::Create( );

    memset( mStateThumbBuffer, 0, sizeof( mStateThumbBuffer ) );
}

void StateSaveState::Destroy( )
{
    StateGameStateBase::Destroy( );
}

void StateSaveState::EnterState( UIState oldState )
{
    StateGameStateBase::EnterState( oldState );

    memset( mStateThumbBuffer, 0, sizeof( mStateThumbBuffer ) );
}

void StateSaveState::ExitState( UIState newState )
{
    StateGameStateBase::ExitState( newState );
}

void StateSaveState_OnComplete( int result, void *pInstance )
{
    ( (StateSaveState *)pInstance )->OnSaveLoadComplete( result );
}

UIState StateSaveState::Update( )
{
    // dont allow input while showing the processing ui
    if ( mSaveLoadThreadState == SaveLoadThreadState_None
        && mProcessingUITimerMS < gGlobalTimer.GetElapsedTimeMicroseconds( ) )
    {
        if ( ShockInput::GetInput( P1_Button_1 )->WasReleased( ) )
        {
            ShowProcessingUI( );
            ShockGame::SaveGameState( mMenuSelection, mStateThumbBuffer, StateSaveState_OnComplete, this );
        }
    }

    // watch for completion
    if ( mSaveLoadThreadState == SaveLoadThreadState_Complete )
    {
        if ( mSaveLoadResult == 1 )
        {
            memcpy( mStateThumb[ mMenuSelection ], mStateThumbBuffer, sizeof( mStateThumbBuffer ) );
            mStateExists[ mMenuSelection ] = 1;
            snprintf( mResultStr, sizeof( mResultStr ), "State for slot %d saved. Return to game to continue.", mMenuSelection + 1 );
        }
        else
        {
            memset( mStateThumbBuffer, 0, sizeof( mStateThumbBuffer ) );
            mStateExists[ mMenuSelection ] = 0;
            snprintf( mResultStr, sizeof( mResultStr ), "Error saving state for slot: %d. Check USB Drive and scan for errors.", mMenuSelection + 1 );
        }

        mSaveLoadThreadState = SaveLoadThreadState_None;
        mSaveLoadResult = 0;
    }

    StateGameStateBase::DrawMenu( "SAVE STATE", "SAVING..." );

    return StateGameStateBase::Update( );
}
