
// See License.md for license

#include "../../includes.h"

void StateMainMenu::Create( )
{
    UIBaseState::Create( );
        
    mExitEmulator = 0;
    mExitUI       = 0;
    mNumMenuItems = 0;

    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    
    mMenuItemGameState[ mNumMenuItems ].menuItem.Create( "Return To Game", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ mNumMenuItems++ ].gameState = UIState_Count;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ mNumMenuItems ].menuItem.Create( "Save State", xPos, yPos, UI_COLOR_DISABLED );
    mMenuItemGameState[ mNumMenuItems++ ].gameState = UIState_SaveState;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ mNumMenuItems ].menuItem.Create( "Load State", xPos, yPos, UI_COLOR_DISABLED );
    mMenuItemGameState[ mNumMenuItems++ ].gameState = UIState_LoadState;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ mNumMenuItems ].menuItem.Create( "Game Settings", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ mNumMenuItems++ ].gameState = UIState_GameSettings;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ mNumMenuItems ].menuItem.Create( "Button Config", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ mNumMenuItems++ ].gameState = UIState_ButtonConfig;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ mNumMenuItems ].menuItem.Create( "Display Settings", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ mNumMenuItems++ ].gameState = UIState_DisplaySettings;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ mNumMenuItems ].menuItem.Create( "Emulator Settings", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ mNumMenuItems++ ].gameState = UIState_EmulatorSettings;
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ mNumMenuItems ].menuItem.Create( "Credits / Legal", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ mNumMenuItems++ ].gameState = UIState_Credits;
 
    yPos += UI_ROW_HEIGHT;
    mMenuItemGameState[ mNumMenuItems ].menuItem.Create( "Quit", xPos, yPos, 0xFFFF );
    mMenuItemGameState[ mNumMenuItems++ ].gameState = UIState_Count;
   
    mMenuSelection = 0;
    
    if ( mNumMenuItems > MAX_MENU_ITEMS )
    {
        flushPrintf( "StateMainMenu::Create() ERROR!!! mNumMenuItems too large at %d." 
                     "MAX_MENU_ITEMS is only: %d\r\n", 
                     mNumMenuItems, 
                     MAX_MENU_ITEMS );
        exit( 0 );
    }
}

void StateMainMenu::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateMainMenu::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );
    
    mExitEmulator = 0;
    mExitUI       = 0;
}

void StateMainMenu::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
    
    mExitEmulator = 0;
    mExitUI       = 0;
}
    
UIState StateMainMenu::Update( )
{
    UIBaseState::Update( );
    
    // draw our menu options
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
    if( ShockInput::GetInput( P1_Red )->WasReleased() )
    {   
        if( mMenuSelection == 0 )
        {
            mExitUI = 1;
        }
        else if ( mMenuSelection == 8 )
        {
            mExitEmulator = 1;
        }
        else
        {
            return mMenuItemGameState[ mMenuSelection ].gameState;
        }
    }
    
    // returning is different vs normal ui states, so don't use the base method
    if( ShockInput::GetInput( OptionsBack )->WasReleased( ) )
    {
        mExitUI = 1;
    }
    
    return UIState_Count;
}

void StateMainMenu::DrawMenu( )
{
    int i = 0;
    for( i = 0; i < mNumMenuItems; i++ )
    {
        mMenuItemGameState[ i ].menuItem.Draw( );
    }
    
    UIRenderer::DrawText( "X", 
                        mMenuItemGameState[ mMenuSelection ].menuItem.GetXPos( ) - UI_CURSOR_X_OFFSET, 
                        mMenuItemGameState[ mMenuSelection ].menuItem.GetYPos( ),
                        UI_COLOR_ENABLED );
                        
    UIBaseState::RenderBackOption( "Return" );
}

int StateMainMenu::ShouldExitUI( )
{
    return mExitUI;
}

int StateMainMenu::ShouldExitEmulator( )
{
    return mExitEmulator;
}
