
// See License.md for license

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

UIState StateLoadState::Update( )
{
    if ( ShockInput::GetInput( P1_Button_1 )->WasReleased( ) )
    {
        if ( mStateExists[ mMenuSelection ] )
        {
            int result = ShockGame::LoadGameState( mMenuSelection );
            if ( result == 0 )
            {
                snprintf( mResultStr, sizeof( mResultStr ), "State for slot %d loaded. Return to game to play.", mMenuSelection + 1 );
            }
            else
            {
                snprintf( mResultStr, sizeof( mResultStr ), "Error loading state for slot: %d", mMenuSelection + 1 );
            }
        }
    }

    return StateGameStateBase::Update( );
}

void StateLoadState::DrawMenu( )
{
    UIBaseState::RenderTitle( "LOAD STATE" );

    StateGameStateBase::DrawMenu( );
}
