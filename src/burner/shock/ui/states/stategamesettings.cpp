
// See License.md for license

#include "shock/input/shockinput.h"
#include "shock/shockgame.h"
#include "shock/ui/render/uirenderer.h"
#include "shock/ui/states/stategamesettings.h"
#include "shock/util/util.h"

void StateGameSettings::Create( )
{
    UIBaseState::Create( );
    
    mNumMenuItems = 0;
    
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    
    mMenuItemList[ mNumMenuItems++ ].Create( "In-Game Settings Menu: ", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ mNumMenuItems++ ].Create( "Restart Game: ", xPos, yPos, 0xFFFF );
    
    mMenuSelection = 0;
    
    if ( mNumMenuItems > MAX_MENU_ITEMS )
    {
        flushPrintf( "StateGameSettings::Create() ERROR!!! mNumMenuItems too large at %d." 
                     "MAX_MENU_ITEMS is only: %d\r\n", 
                     mNumMenuItems, 
                     MAX_MENU_ITEMS );
        exit( 0 );
    }
}

void StateGameSettings::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateGameSettings::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );

    mMenuSelection = 0;
}

void StateGameSettings::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
}

UIState StateGameSettings::Update( )
{
    UIBaseState::Update( );
    
    DrawMenu( );
    
    // check for menu navigation
    if( ShockInput::GetInput( P1_Joy_Down )->WasReleased() )
    {
        mMenuSelection = (mMenuSelection + 1) % mNumMenuItems;
    }
    else if( ShockInput::GetInput( P1_Joy_Up )->WasReleased() )
    {
        mMenuSelection--;
        if( mMenuSelection < 0 )
        {
            mMenuSelection = mNumMenuItems - 1;
        }
    }
        
    // check for entering a gamestate menu item
    if( ShockInput::GetInput( P1_Button_1 )->WasReleased( ) )
    {   
        if( mMenuSelection == 0 )
        {
            if ( ShockGame::GameHasDiagnosticMode( ) )
            {
                int enabled = !ShockGame::GetDiagnosticMode( );
                ShockGame::SetDiagnosticMode( enabled );
            }
        }
        else if( mMenuSelection == 1 )
        {
            if (ShockGame::GameHasReset( ) )
            {
                int enabled = !ShockGame::GetReset( );
                ShockGame::SetReset( enabled );
            }
        }
    }    
    
    // should we exit?
    return UIBaseState::HandleBackButton( );
}

void StateGameSettings::DrawMenu( )
{   
    UIBaseState::RenderTitle( "GAME SETTINGS" );
    
    // Diagnostic Mode
    mMenuItemList[ 0 ].Draw( );
    
    char settingStr[ MAX_PATH ] = { 0 };
    int menuItemLen = 0;
    
    UINT16 settingColor = 0xFFFF;
    if( ShockGame::GameHasDiagnosticMode( ) )
    {
        if ( ShockGame::GetDiagnosticMode( ) == 1 )
        {
            strncpy( settingStr, "On", sizeof( settingStr ) - 1 );
            settingColor = UI_COLOR_ENABLED;
        }
        else
        {
            strncpy( settingStr, "Off", sizeof( settingStr ) - 1 );
            settingColor = 0xFFFF;
        }
    }
    else
    {
        settingColor = UI_COLOR_DISABLED;
        strncpy( settingStr, "Not Available", sizeof( settingStr ) - 1 );
    }
    menuItemLen = Font::MeasureStringWidth( mMenuItemList[ 0 ].GetText( ) );
    UIRenderer::DrawText( settingStr, mMenuItemList[ 0 ].GetXPos( ) + menuItemLen, mMenuItemList[ 0 ].GetYPos( ), settingColor );
    //
    
    // Reset Mode
    mMenuItemList[ 1 ].Draw( );
    
    if( ShockGame::GameHasReset( ) )
    {
        if ( ShockGame::GetReset( ) == 1 )
        {
            strncpy( settingStr, "On", sizeof( settingStr ) - 1 );
            settingColor = UI_COLOR_ENABLED;
        }
        else
        {
            strncpy( settingStr, "Off", sizeof( settingStr ) - 1 );
            settingColor = 0xFFFF;
        }
    }
    else
    {
        settingColor = UI_COLOR_DISABLED;
        strncpy( settingStr, "Not Available", sizeof( settingStr ) - 1 );
    }
    menuItemLen = Font::MeasureStringWidth( mMenuItemList[ 1 ].GetText( ) );
    UIRenderer::DrawText( settingStr, mMenuItemList[ 1 ].GetXPos( ) + menuItemLen, mMenuItemList[ 1 ].GetYPos( ), settingColor );
    //
    
    // render a note about settings being applied
    char noteStr[ MAX_PATH ] = { 0 };
    strncpy( noteStr, "Return to game to activate settings", sizeof( noteStr ) - 1 );
    int xPos = UIBaseState::GetCenteredXPos( noteStr );
    int yPos = mMenuItemList[ 1 ].GetYPos( ) + 50;
    UIRenderer::DrawText( noteStr, UI_X_POS_MENU, yPos, 0xFFFF );
    
    // seperator
    yPos += 100;
    int lineWidth = 200;
    xPos = (UI_WIDTH - lineWidth) / 2;
    UIRenderer::DrawLine( 0xFFFF, xPos, yPos, lineWidth );
    
    // dips
    yPos += 10;
    strncpy( noteStr, "Dip Switch Support Coming Soon", sizeof( noteStr ) - 1 );
    xPos = UIBaseState::GetCenteredXPos( noteStr );
    UIRenderer::DrawText( noteStr, 
                        xPos, 
                        yPos, 
                        0xFFFF );
    
    UIBaseState::RenderMenuCursor( mMenuItemList[ mMenuSelection ].GetXPos( ), 
                                   mMenuItemList[ mMenuSelection ].GetYPos( ) );
                        
    UIBaseState::RenderBackOption( "Return" );
}
