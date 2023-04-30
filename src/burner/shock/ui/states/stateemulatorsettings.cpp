
// See License.md for license

#include "../../includes.h"

void StateEmulatorSettings::Create( )
{
    UIBaseState::Create( );
    
    mNumMenuItems = 0;
    
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    mMenuItemList[ mNumMenuItems++ ].Create( "Display FPS Counter", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ mNumMenuItems++ ].Create( "Display CRC Warning", xPos, yPos, 0xFFFF );
    
    memset( mResultStr, 0, sizeof( mResultStr ) );
    
    mMenuSelection = 0;
    
    if ( mNumMenuItems > MAX_MENU_ITEMS )
    {
        flushPrintf( "StateEmulatorSettings::Create() ERROR!!! mNumMenuItems too large at %d. MAX_MENU_ITEMS is only: %d\r\n", mNumMenuItems, MAX_MENU_ITEMS );
        exit( 0 );
    }
}

void StateEmulatorSettings::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateEmulatorSettings::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );
        
    // reset our output
    memset( mResultStr, 0, sizeof( mResultStr ) );
}

void StateEmulatorSettings::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
}

UIState StateEmulatorSettings::Update( )
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
    if( ShockInput::GetInput( P1_Red )->WasReleased() )
    {   
        if( mMenuSelection == 0 )
        {
            ShockConfig::SetShowFPS( !ShockConfig::GetShowFPS( ) );
        }
        else if ( mMenuSelection == 1 )
        {
        }
    }    
    
    // should we exit?
    return UIBaseState::HandleBackButton( );
}

void StateEmulatorSettings::DrawMenu( )
{
    short textColor = ShockConfig::GetShowFPS( ) == 1 ? textColor = UI_COLOR_ENABLED : 0xFFFF;
    mMenuItemList[0].SetColor( textColor );
    mMenuItemList[0].Draw( );
    
    
    textColor = 0xFFFF;//ShockConfig::GetDisplayMode() == 1 ? textColor = 0x87e0 : 0xFFFF;
    mMenuItemList[1].SetColor( textColor );
    mMenuItemList[1].Draw( );
    
    
    UIRenderer::DrawText( "X", 
                        mMenuItemList[ mMenuSelection ].GetXPos( ) - UI_CURSOR_X_OFFSET, 
                        mMenuItemList[ mMenuSelection ].GetYPos( ),
                        UI_COLOR_ENABLED );
    
    UIBaseState::RenderBackOption( );
}
