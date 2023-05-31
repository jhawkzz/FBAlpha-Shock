
// See License.md for license

#include "shock/input/shockinput.h"
#include "shock/ui/states/uibasestate.h"
#include "shock/ui/render/uirenderer.h"

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
    return UIState_Count;
}

UIState UIBaseState::HandleBackButton( )
{
    if( ShockInput::GetInput( P1_InsertCoin )->WasReleased( ) )
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
                          menuX - Font::MeasureStringWidth( "X" ) - 5, 
                          menuY,
                          UI_COLOR_ENABLED );
}

int UIBaseState::GetCenteredXPos( const char *pText, FontType fontType/*= FontType_Met*/ )
{
    int width = Font::MeasureStringWidth( pText, fontType );
    return (PLATFORM_LCD_WIDTH - width) / 2;
}

void UIBaseState::RenderTitle( const char *pText )
{
    int xPos = 0;
    int yPos = UI_Y_POS_HEADER_TITLE;
    
    char message[ MAX_PATH ] = { 0 };
    strncpy( message, pText, sizeof( message ) - 1 );
    xPos = GetCenteredXPos( message, FontType_Upheaval );
    UIRenderer::DrawText( message, xPos, yPos, 0xFFFF, FontType_Upheaval );
}
