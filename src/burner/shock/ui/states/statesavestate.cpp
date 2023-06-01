
// See License.md for license

#include "shock/ui/states/statesavestate.h"
#include "shock/input/shockinput.h"
#include "shock/shockgame.h"

void StateSaveState::Create( )
{
    StateGameStateBase::Create( );
}

void StateSaveState::Destroy( )
{
    StateGameStateBase::Destroy( );
}

void StateSaveState::EnterState( UIState oldState )
{
    StateGameStateBase::EnterState( oldState );
}

void StateSaveState::ExitState( UIState newState )
{
    StateGameStateBase::ExitState( newState );
}

UIState StateSaveState::Update( )
{
    // check for entering a gamestate menu item
    if (ShockInput::GetInput(P1_Button_1)->WasReleased())
    {
        int result = ShockGame::SaveGameState(mMenuSelection, mStateThumb[mMenuSelection]);
        if (result == -1)
        {
            memset(mStateThumb[mMenuSelection], 0, sizeof(mStateThumb[mMenuSelection]));
            mStateExists[mMenuSelection] = 0;
            snprintf(mResultStr, sizeof(mResultStr), "Error creating save state for slot: %d", mMenuSelection + 1);
        }
        else
        {
            mStateExists[mMenuSelection] = 1;
            memset(mResultStr, 0, sizeof(mResultStr));
        }
    }

    return StateGameStateBase::Update( );
}

void StateSaveState::DrawMenu( )
{
    UIBaseState::RenderTitle( "SAVE STATE" );
    
    StateGameStateBase::DrawMenu();
}
