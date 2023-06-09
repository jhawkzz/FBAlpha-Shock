
// See License.md for license

#include "shock/input/shockinput.h"
#include "shock/shockconfig.h"
#include "shock/ui/render/uirenderer.h"
#include "shock/ui/states/stateemulatorsettings.h"
#include "shock/util/util.h"

void StateEmulatorSettings::Create( )
{
    UIBaseState::Create( );
    
    mNumMenuItems = 0;
    
    mFpsMenu = &mMenuItemList[ mNumMenuItems++ ];
    mTimerMenu = &mMenuItemList[ mNumMenuItems++ ];
    mDisplayMenu = &mMenuItemList[ mNumMenuItems++ ];

    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    mFpsMenu->Create( "Display FPS Counter: ", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    mTimerMenu->Create( "Display Timers: ", xPos, yPos, 0xFFFF );

    yPos += UI_ROW_HEIGHT;
    mDisplayMenu->Create( "Display Load Warnings: ", xPos, yPos, 0xFFFF );
    
    mMenuSelection = 0;
    
    if ( mNumMenuItems > MAX_MENU_ITEMS )
    {
        flushPrintf( "StateEmulatorSettings::Create() ERROR!!! mNumMenuItems too large at %d." 
                     "MAX_MENU_ITEMS is only: %d\r\n", 
                     mNumMenuItems, 
                     MAX_MENU_ITEMS );
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
    if( ShockInput::GetInput( P1_Button_1 )->WasReleased() )
    {   
        MenuItem* selected = &mMenuItemList[ mMenuSelection ];

        if( mFpsMenu ==  selected )
        {
            ShockConfig::SetShowFPS( !ShockConfig::GetShowFPS( ) );
        }
        else if( mTimerMenu == selected )
        {
            ShockConfig::SetShowTimers( !ShockConfig::GetShowTimers( ) );
        }
        else if( mDisplayMenu == selected )
        {
            ShockConfig::SetShowLoadWarnings( !ShockConfig::GetShowLoadWarnings( ) );
        }
    }    
    
    // should we exit?
    return UIBaseState::HandleBackButton( );
}

void StateEmulatorSettings::DrawMenu( )
{
    UIBaseState::RenderTitle( "EMULATOR SETTINGS" );
    
    char settingStr[ MAX_PATH ] = { 0 };
    short textColor = 0;
    int menuItemLen = 0;
    
    auto showMenu = [&](MenuItem* item, bool enabled)
    {
        item->Draw( );
        if (enabled)
        {
            strncpy( settingStr, "On", sizeof( settingStr ) - 1 );
            textColor = UI_COLOR_ENABLED;
        }
        else
        {
            strncpy( settingStr, "Off", sizeof( settingStr ) - 1 );
            textColor = 0xFFFF;
        }

        menuItemLen = Font::MeasureStringWidth( item->GetText( ) );
        UIRenderer::DrawText( settingStr, item->GetXPos( ) + menuItemLen, item->GetYPos( ), textColor );
    };

    showMenu(mFpsMenu, ShockConfig::GetShowFPS( ) != 0);
    showMenu(mTimerMenu, false);
    showMenu(mDisplayMenu, ShockConfig::GetShowLoadWarnings() != 0);
    
    UIRenderer::DrawText( "X", 
                        mMenuItemList[ mMenuSelection ].GetXPos( ) - UI_CURSOR_X_OFFSET, 
                        mMenuItemList[ mMenuSelection ].GetYPos( ),
                        UI_COLOR_ENABLED );
    
    UIBaseState::RenderBackOption( "Return" );
}
