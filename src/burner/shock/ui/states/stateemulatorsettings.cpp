
// See License.md for license

#include "shock/input/shockinput.h"
#include "shock/shockconfig.h"
#include "shock/ui/render/uirenderer.h"
#include "shock/ui/states/stateemulatorsettings.h"
#include "shock/util/util.h"

void StateEmulatorSettings::Create( )
{
    UIBaseState::Create( );
    
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    mMenuItemList[ (UINT32) MenuIdFps ].Create( "Display FPS Counter: ", xPos, yPos, 0xFFFFu );
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ (UINT32) MenuIdLoadWarnings ].Create( "Display Load Warnings: ", xPos, yPos, 0xFFFFu );
    
#ifdef SHOCK_PROFILERS
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ (UINT32) MenuIdTimers ].Create( "Display Timers: ", xPos, yPos, 0xFFFFU );
#endif

    mMenuSelection = MenuIdFps;
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
        mMenuSelection = (MenuId)(((UINT32)mMenuSelection + 1) % (UINT32)MenuIdCount);
    }
    else if( ShockInput::GetInput( P1_Joy_Up )->WasReleased() )
    {
        mMenuSelection = (MenuId)((UINT32)mMenuSelection - 1);
        if( mMenuSelection < MenuIdStart )
        {
            mMenuSelection = (MenuId)(((UINT32)MenuIdCount) - 1);
        }
    }
        
    // check for entering a gamestate menu item
    if( ShockInput::GetInput( P1_Button_1 )->WasReleased() )
    {   
        if( mMenuSelection == MenuIdFps )
        {
            ShockConfig::SetShowFPS( !ShockConfig::GetShowFPS( ) );
        }
        else if ( mMenuSelection == MenuIdLoadWarnings )
        {
            ShockConfig::SetShowLoadWarnings( !ShockConfig::GetShowLoadWarnings( ) );
        }
#ifdef SHOCK_PROFILERS
        else if ( mMenuSelection == MenuIdTimers )
        {
            ShockConfig::SetShowTimers( !ShockConfig::GetShowTimers( ) );
        }
#endif
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
    
    // FPS
    {
        mMenuItemList[ (UINT32)MenuIdFps ].Draw( );
        if ( ShockConfig::GetShowFPS( ) == 1 )
        {
            strncpy( settingStr, "On", sizeof( settingStr ) - 1 );
            textColor = UI_COLOR_ENABLED;
        }
        else
        {
            strncpy( settingStr, "Off", sizeof( settingStr ) - 1 );
            textColor = 0xFFFFu;
        }
    
        menuItemLen = Font::MeasureStringWidth( mMenuItemList[ (UINT32)MenuIdFps ].GetText( ) );
        UIRenderer::DrawText( settingStr, mMenuItemList[ (UINT32)MenuIdFps ].GetXPos( ) + menuItemLen, mMenuItemList[ (UINT32)MenuIdFps ].GetYPos( ), textColor );
    }

    // Load Warnings
    {
        mMenuItemList[ (UINT32)MenuIdLoadWarnings ].Draw( );
        if ( ShockConfig::GetShowLoadWarnings( ) == 1 )
        {
            strncpy( settingStr, "On", sizeof( settingStr ) - 1 );
            textColor = UI_COLOR_ENABLED;
        }
        else
        {
            strncpy( settingStr, "Off", sizeof( settingStr ) - 1 );
            textColor = 0xFFFFu;
        }
    
        menuItemLen = Font::MeasureStringWidth( mMenuItemList[ (UINT32)MenuIdLoadWarnings ].GetText( ) );
        UIRenderer::DrawText( settingStr, mMenuItemList[ (UINT32)MenuIdLoadWarnings ].GetXPos( ) + menuItemLen, mMenuItemList[ (UINT32)MenuIdLoadWarnings ].GetYPos( ), textColor );
    }

    // Timers
#ifdef SHOCK_PROFILERS
    {
        mMenuItemList[ (UINT32)MenuIdTimers ].Draw( );
        if ( ShockConfig::GetShowTimers( ) == 1 )
        {
            strncpy( settingStr, "On", sizeof( settingStr ) - 1 );
            textColor = UI_COLOR_ENABLED;
        }
        else
        {
            strncpy( settingStr, "Off", sizeof( settingStr ) - 1 );
            textColor = 0xFFFFu;
        }

        menuItemLen = Font::MeasureStringWidth( mMenuItemList[ (UINT32)MenuIdTimers ].GetText( ) );
        UIRenderer::DrawText( settingStr, mMenuItemList[ (UINT32)MenuIdTimers ].GetXPos( ) + menuItemLen, mMenuItemList[ (UINT32)MenuIdTimers ].GetYPos( ), textColor );
    }
#endif

    UIRenderer::DrawText( "X", 
                        mMenuItemList[ (UINT32)mMenuSelection ].GetXPos( ) - UI_CURSOR_X_OFFSET, 
                        mMenuItemList[ (UINT32)mMenuSelection ].GetYPos( ),
                        UI_COLOR_ENABLED );
    
    UIBaseState::RenderBackOption( "Return" );
}
