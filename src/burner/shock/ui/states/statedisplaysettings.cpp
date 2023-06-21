
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
    mMenuItemList[ mNumMenuItems++ ].Create( "Aspect Ratio (Full Screen)", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT;
    mMenuItemList[ mNumMenuItems++ ].Create( "Full Screen", xPos, yPos, 0xFFFF );
    
    yPos += UI_ROW_HEIGHT * 3;
    mMenuItemList[ mNumMenuItems++ ].Create( "Display Filter: ", xPos, yPos, 0xFFFF );
    
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
        if ( mMenuSelection == 0 )
        {
            ShockConfig::SetDisplayMode( ShockDisplayMode_AspectRatio );
        }
        else if ( mMenuSelection == 1 )
        {
            ShockConfig::SetDisplayMode( ShockDisplayMode_FullScreen );
        }
        else if ( mMenuSelection == 2 )
        {
            int displayFilter = ShockConfig::GetDisplayFilter( );
            displayFilter = (displayFilter + 1) % ShockDisplayFilter_Count;
            ShockConfig::SetDisplayFilter( displayFilter );
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
    
    // seperator
    int lineWidth = 200;
    int xPos = ( UI_WIDTH - lineWidth ) / 2;
    int yPos = mMenuItemList[ 2 ].GetYPos( ) + 50;
    UIRenderer::DrawLine( 0xFFFF, xPos, yPos, lineWidth );
                              
    // Display Filter
    char displayStr[ MAX_PATH ] = { 0 };
    char explanationOneStr[ MAX_PATH ] = { 0 };
    char explanationTwoStr[ MAX_PATH ] = { 0 };
    char explanationThreeStr[ MAX_PATH ] = { 0 };
    int menuItemLen = 0;
    
    mMenuItemList[ 2 ].Draw( );
    switch ( ShockConfig::GetDisplayFilter( ) )
    {
        case ShockDisplayFilter_Pixel:
        {
            strncpy( displayStr, "Pixel Perfect", sizeof( displayStr ) - 1 );
            strncpy( explanationOneStr, "Original pixel art with no filtering", sizeof( explanationOneStr ) - 1 );
            strncpy( explanationTwoStr, "Most games run at 60fps. Some will run slower", sizeof( explanationTwoStr ) - 1 );
            break;
        }

        case ShockDisplayFilter_Pixel_Scanline:
        {
            strncpy( displayStr, "Pixel Perfect Scanlines", sizeof( displayStr ) - 1 );
            strncpy( explanationOneStr, "Original pixel art with simulated scanlines", sizeof( explanationOneStr ) - 1 );
            strncpy( explanationTwoStr, "Most games run at 60fps. Some will run slower", sizeof( explanationTwoStr ) - 1 );
            break;
        }

        case ShockDisplayFilter_Smoothing:
        {
            strncpy( displayStr, "Pixel Smoothing", sizeof( displayStr ) - 1 );
            strncpy( explanationOneStr, "Smooths the pixel art to simulate a crt look", sizeof( explanationOneStr ) - 1 );
            strncpy( explanationTwoStr, "Most games run at 60 fps. Some will run slower", sizeof( explanationTwoStr ) - 1 );
            strncpy( explanationThreeStr, "You may need to adjust your tv display setting to 4:3", sizeof( explanationThreeStr ) - 1 );
            break;
        }

        case ShockDisplayFilter_Performance:
        {
            strncpy( displayStr, "Performance", sizeof( displayStr ) - 1 );
            strncpy( explanationOneStr, "Extremely high fps but a slightly soft image", sizeof( explanationOneStr ) - 1 );
            strncpy( explanationTwoStr, "All games run at or near 60 fps", sizeof( explanationTwoStr ) - 1 );
            strncpy( explanationThreeStr, "You may need to adjust your tv display setting to 4:3", sizeof( explanationThreeStr ) - 1 );
            break;
        }
    }
    menuItemLen = Font::MeasureStringWidth( mMenuItemList[ 2 ].GetText( ) );
    UIRenderer::DrawText( displayStr, mMenuItemList[ 2 ].GetXPos( ) + menuItemLen, mMenuItemList[ 2 ].GetYPos( ), UI_COLOR_ENABLED );

    yPos = mMenuItemList[ 2 ].GetYPos( ) + UI_ROW_HEIGHT * 2;
    UIRenderer::DrawText( explanationOneStr, mMenuItemList[ 2 ].GetXPos( ), yPos, 0xFFFF );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( explanationTwoStr, mMenuItemList[ 2 ].GetXPos( ), yPos, 0xFFFF );

    if ( ActivePlatform_ASP == gActivePlatform )
    {
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( explanationThreeStr, mMenuItemList[ 2 ].GetXPos( ), yPos + UI_ROW_HEIGHT / 2, 0xFFFF );
    }
    
    // Cursor
    UIBaseState::RenderMenuCursor( mMenuItemList[ mMenuSelection ].GetXPos( ), 
                                   mMenuItemList[ mMenuSelection ].GetYPos( ) );
    
    UIBaseState::RenderBackOption( "Return" );
}
