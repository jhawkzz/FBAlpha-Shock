
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
    mMenuItemList[ (UINT32) MenuIndex::Fps ].Create( "Display FPS Counter: ", xPos, yPos, 0xFFFFu );
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ (UINT32) MenuIndex::LoadWarnings ].Create( "Display Load Warnings: ", xPos, yPos, 0xFFFFu );
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ (UINT32) MenuIndex::Timers ].Create( "Display Timers: ", xPos, yPos, 0xFFFFU );

    mMenuSelection = MenuIndex::Fps;
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
        mMenuSelection = (MenuIndex)(((UINT32)mMenuSelection + 1) % (UINT32)MenuIndex::Count);
    }
    else if( ShockInput::GetInput( P1_Joy_Up )->WasReleased() )
    {
        mMenuSelection = (MenuIndex)((UINT32)mMenuSelection - 1);
        if( mMenuSelection < MenuIndex::Start )
        {
            mMenuSelection = (MenuIndex)(((UINT32)MenuIndex::Count) - 1);
        }
    }
        
    // check for entering a gamestate menu item
    if( ShockInput::GetInput( P1_Button_1 )->WasReleased() )
    {   
        if( mMenuSelection == MenuIndex::Fps )
        {
            ShockConfig::SetShowFPS( !ShockConfig::GetShowFPS( ) );
        }
        else if ( mMenuSelection == MenuIndex::LoadWarnings )
        {
            ShockConfig::SetShowLoadWarnings( !ShockConfig::GetShowLoadWarnings( ) );
        }
        else if ( mMenuSelection == MenuIndex::Timers )
        {
            ShockConfig::SetShowTimers( !ShockConfig::GetShowTimers( ) );
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
    
    // FPS
    {
        mMenuItemList[ (UINT32)MenuIndex::Fps ].Draw( );
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
    
        menuItemLen = Font::MeasureStringWidth( mMenuItemList[ (UINT32)MenuIndex::Fps ].GetText( ) );
        UIRenderer::DrawText( settingStr, mMenuItemList[ (UINT32)MenuIndex::Fps ].GetXPos( ) + menuItemLen, mMenuItemList[ (UINT32)MenuIndex::Fps ].GetYPos( ), textColor );
    }

    // Load Warnings
    {
        mMenuItemList[ (UINT32)MenuIndex::LoadWarnings ].Draw( );
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
    
        menuItemLen = Font::MeasureStringWidth( mMenuItemList[ (UINT32)MenuIndex::LoadWarnings ].GetText( ) );
        UIRenderer::DrawText( settingStr, mMenuItemList[ (UINT32)MenuIndex::LoadWarnings ].GetXPos( ) + menuItemLen, mMenuItemList[ (UINT32)MenuIndex::LoadWarnings ].GetYPos( ), textColor );
    }

    // Timers
    {
        mMenuItemList[ (UINT32)MenuIndex::Timers ].Draw( );
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

        menuItemLen = Font::MeasureStringWidth( mMenuItemList[ (UINT32)MenuIndex::Timers ].GetText( ) );
        UIRenderer::DrawText( settingStr, mMenuItemList[ (UINT32)MenuIndex::Timers ].GetXPos( ) + menuItemLen, mMenuItemList[ (UINT32)MenuIndex::Timers ].GetYPos( ), textColor );
    }

    UIRenderer::DrawText( "X", 
                        mMenuItemList[ (UINT32)mMenuSelection ].GetXPos( ) - UI_CURSOR_X_OFFSET, 
                        mMenuItemList[ (UINT32)mMenuSelection ].GetYPos( ),
                        UI_COLOR_ENABLED );
    
    UIBaseState::RenderBackOption( "Return" );
}
