
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

void UIBaseState::RenderBackOption( const char *pNavVerb)
{
    char textStr[ MAX_PATH ] = { 0 };
    snprintf( textStr, sizeof( textStr ), "Press Options/Back to %s", pNavVerb );
    int xPos = GetCenteredXPos( textStr );
    UIRenderer::DrawText( textStr, xPos, PLATFORM_LCD_HEIGHT - 50, 0xFFFF );
}

void UIBaseState::RenderMenuCursor( int menuX, int menuY )
{
    UIRenderer::DrawText( "X", 
                          menuX - Font::MeasureStringWidth( "X" ), 
                          menuY,
                          UI_COLOR_ENABLED );
}

int UIBaseState::GetCenteredXPos( const char *pText )
{
    int width = Font::MeasureStringWidth( pText );
    return (PLATFORM_LCD_WIDTH - width) / 2;
}
