
// See License.md for license

#include "../../includes.h"

void UIBaseState::Create( )
{
}

void UIBaseState::Destroy( )
{
}

void UIBaseState::EnterState( UIState oldState )
{
    mLastState = oldState;
}

void UIBaseState::ExitState( UIState newState )
{
}
    
UIState UIBaseState::Update( )
{
}

UIState UIBaseState::HandleBackButton( )
{
    if( ShockInput::GetInput( OptionsBack )->WasReleased( ) )
    {
        return mLastState;
    }
    else
    {
        return UIState_Count;
    }
}

void UIBaseState::RenderBackOption( )
{
    UIRenderer::DrawText( "Press Options/Back to Return", UI_X_POS_MENU, PLATFORM_LCD_HEIGHT - 50, 0xFFFF );
}
