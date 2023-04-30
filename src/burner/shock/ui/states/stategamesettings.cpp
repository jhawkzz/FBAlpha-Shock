
// See License.md for license

#include "../../includes.h"

void StateGameSettings::Create( )
{
    UIBaseState::Create( );
    
    mNumMenuItems = 0;
    
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    
    mMenuItemList[ mNumMenuItems++ ].Create( "Enter Game Settings Menu", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT * 2;
    mMenuItemList[ mNumMenuItems++ ].Create( "Reset Game", xPos, yPos, 0xFFFF );
    
    mMenuSelection = 0;
    
    if ( mNumMenuItems > MAX_MENU_ITEMS )
    {
        flushPrintf( "StateGameSettings::Create() ERROR!!! mNumMenuItems too large at %d. MAX_MENU_ITEMS is only: %d\r\n", mNumMenuItems, MAX_MENU_ITEMS );
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
    if( ShockInput::GetInput( P1_Red )->WasReleased( ) )
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
    // Diagnostic Mode
    int textColor = 0xFFFF;
    if( ShockGame::GameHasDiagnosticMode( ) && ShockGame::GetDiagnosticMode( ) )
    {
        textColor = UI_COLOR_ENABLED;
        UIRenderer::DrawText( "Game Settings Menu Will Be Entered When Game Is Resumed", 
                        UI_X_POS_MENU, 
                        mMenuItemList[ 0 ].GetYPos( ) + UI_ROW_HEIGHT / 2, 
                        0xFFFF );
    }
    else if( ShockGame::GameHasDiagnosticMode( ) == 0 )
    {
        textColor = UI_COLOR_DISABLED;
        UIRenderer::DrawText( "This Game Does Not Have A Settings Menu", 
                        UI_X_POS_MENU, 
                        mMenuItemList[ 0 ].GetYPos( ) + UI_ROW_HEIGHT / 2, 
                        0xFFFF );
    }
    
    mMenuItemList[ 0 ].SetColor( textColor );
    mMenuItemList[ 0 ].Draw( );
    
    // Reset Mode
    textColor = 0xFFFF;
    if( ShockGame::GameHasReset( ) && ShockGame::GetReset( ) )
    {
        textColor = UI_COLOR_ENABLED;
        UIRenderer::DrawText( "Game Will Be Reset When Game Is Resumed", 
                        UI_X_POS_MENU, 
                        mMenuItemList[ 1 ].GetYPos( ) + UI_ROW_HEIGHT / 2, 
                        0xFFFF );
    }
    else if( ShockGame::GameHasReset( ) == 0 )
    {
        textColor = UI_COLOR_DISABLED;
        UIRenderer::DrawText( "This Game Does Not Have A Reset Option", 
                        UI_X_POS_MENU, 
                        mMenuItemList[ 1 ].GetYPos( ) + UI_ROW_HEIGHT / 2, 
                        0xFFFF );
    }
    
    mMenuItemList[ 1 ].SetColor( textColor );
    mMenuItemList[ 1 ].Draw( );
    
    UIRenderer::DrawText( "Dip Switch Support Coming Soon", 
                        UI_X_POS_MENU, 
                        mMenuItemList[ 1 ].GetYPos( ) + UI_ROW_HEIGHT * 2, 
                        0xFFFF );
    
    UIRenderer::DrawText( "X", 
                        mMenuItemList[ mMenuSelection ].GetXPos( ) - UI_CURSOR_X_OFFSET, 
                        mMenuItemList[ mMenuSelection ].GetYPos( ),
                        UI_COLOR_ENABLED );
                        
    UIBaseState::RenderBackOption( );
}
