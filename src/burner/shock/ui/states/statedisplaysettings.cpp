
// See License.md for license

#include "shock/input/shockinput.h"
#include "shock/shockconfig.h"
#include "shock/shockrenderer.h"
#include "shock/ui/render/uirenderer.h"
#include "shock/ui/states/statedisplaysettings.h"
#include "shock/util/util.h"

void StateDisplaySettings::Create( )
{
    UIBaseState::Create( );
    
    mNumMenuItems = 0;
    
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU;
    mMenuItemList[ mNumMenuItems++ ].Create( "Original", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ mNumMenuItems++ ].Create( "Original 2x Scaled", xPos, yPos, 0xFFFF );

    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ mNumMenuItems++ ].Create( "Aspect Ratio (Full Screen)", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ mNumMenuItems++ ].Create( "Full Screen", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT * 3;
    mMenuItemList[ mNumMenuItems++ ].Create( "Scanlines: ", xPos, yPos, 0xFFFF );

    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ mNumMenuItems++ ].Create( "Pixel Smoothing: ", xPos, yPos, 0xFFFF );
    
    mEnabledSettings = 0;
    mMenuSelection = 0;
    
    if ( mNumMenuItems > MAX_MENU_ITEMS )
    {
        flushPrintf( "StateDisplaySettings::Create() ERROR!!! mNumMenuItems too large at %d." 
                     "MAX_MENU_ITEMS is only: %d\r\n", 
                     mNumMenuItems, 
                     MAX_MENU_ITEMS );
        exit( 0 );
    }
}

void StateDisplaySettings::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateDisplaySettings::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );
        
    mEnabledSettings = 0;
}

void StateDisplaySettings::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
}

UIState StateDisplaySettings::Update( )
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
        if( mMenuSelection == 0 )
        {
            ShockConfig::SetDisplayMode( ShockDisplayMode_Original );
        }
        else if ( mMenuSelection == 1 )
        {
            ShockConfig::SetDisplayMode( ShockDisplayMode_Original2x );
        }
        else if ( mMenuSelection == 2 )
        {
            ShockConfig::SetDisplayMode( ShockDisplayMode_AspectRatio );
        }
        else if ( mMenuSelection == 3 )
        {
            ShockConfig::SetDisplayMode( ShockDisplayMode_FullScreen );
        }
        else if ( mMenuSelection == 4 )
        {
            ShockConfig::SetScanLinesEnabled( !ShockConfig::GetScanLinesEnabled( ) );
        }
        else if ( mMenuSelection == 5 )
        {
            ShockConfig::SetSmoothingEnabled( !ShockConfig::GetSmoothingEnabled( ) );
        }
    }    
    
    // should we exit?
    return UIBaseState::HandleBackButton( );
}

void StateDisplaySettings::DrawMenu( )
{
    UIBaseState::RenderTitle( "DISPLAY SETTINGS" );
    
    short textColor = ShockConfig::GetDisplayMode() == 0 ? textColor = UI_COLOR_ENABLED : 0xFFFF;
    mMenuItemList[0].SetColor( textColor );
    mMenuItemList[0].Draw( );
    
    textColor = ShockConfig::GetDisplayMode() == 1 ? textColor = UI_COLOR_ENABLED : 0xFFFF;
    mMenuItemList[1].SetColor( textColor );
    mMenuItemList[1].Draw( );
    
    textColor = ShockConfig::GetDisplayMode() == 2 ? textColor = UI_COLOR_ENABLED : 0xFFFF;
    mMenuItemList[2].SetColor( textColor );
    mMenuItemList[2].Draw( );

    textColor = ShockConfig::GetDisplayMode( ) == 3 ? textColor = UI_COLOR_ENABLED : 0xFFFF;
    mMenuItemList[ 3 ].SetColor( textColor );
    mMenuItemList[ 3 ].Draw( );
    
    UIRenderer::DrawText( "Filters", 
                          305, // centers it on screen
                          mMenuItemList[ 4 ].GetYPos() - UI_ROW_HEIGHT, 
                          0xFFFF );
                          
    // Scanlines
    char settingStr[ MAX_PATH ] = { 0 };
    int menuItemLen = 0;
    
    mMenuItemList[ 4 ].Draw( );
    if ( ShockConfig::GetScanLinesEnabled( ) == 1 )
    {
        strncpy( settingStr, "On", sizeof( settingStr ) - 1 );
        textColor = UI_COLOR_ENABLED;
    }
    else
    {
        strncpy( settingStr, "Off", sizeof( settingStr ) - 1 );
        textColor = 0xFFFF;
    }
    menuItemLen = Font::MeasureStringWidth( mMenuItemList[ 4 ].GetText( ) );
    UIRenderer::DrawText( settingStr, mMenuItemList[ 4 ].GetXPos( ) + menuItemLen, mMenuItemList[ 4 ].GetYPos( ), textColor );

    // Smoothing
    mMenuItemList[5 ].Draw( );
    if ( ShockConfig::GetSmoothingEnabled( ) == 1 )
    {
        strncpy( settingStr, "On", sizeof( settingStr ) - 1 );
        textColor = UI_COLOR_ENABLED;
    }
    else
    {
        strncpy( settingStr, "Off", sizeof( settingStr ) - 1 );
        textColor = 0xFFFF;
    }
    menuItemLen = Font::MeasureStringWidth( mMenuItemList[ 5 ].GetText( ) );
    UIRenderer::DrawText( settingStr, mMenuItemList[ 5 ].GetXPos( ) + menuItemLen, mMenuItemList[ 5 ].GetYPos( ), textColor );
    
    // Cursor
    UIBaseState::RenderMenuCursor( mMenuItemList[ mMenuSelection ].GetXPos( ), 
                                   mMenuItemList[ mMenuSelection ].GetYPos( ) );
    
    UIBaseState::RenderBackOption( "Return" );
}
