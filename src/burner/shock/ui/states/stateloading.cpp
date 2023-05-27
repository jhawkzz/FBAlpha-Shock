
// See License.md for license

#include "shock/core/ostimer.h"
#include "shock/defines.h"
#include "shock/shockromloader.h"
#include "shock/ui/imagebinaries.h"
#include "shock/ui/render/uirenderer.h"
#include "shock/ui/states/stateloading.h"

void StateLoading::Create( )
{
    UIBaseState::Create( );
    
    mLoadingColorLetterIndex = 0;
    mLoadingTimerMS          = 0;
}

void StateLoading::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateLoading::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );
    
    mLoadingColorLetterIndex = -1;
    mLoadingTimerMS          = gGlobalTimer.GetElapsedTimeMicroseconds();
}

void StateLoading::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
}

UIState StateLoading::Update( )
{
    UIBaseState::Update( );
    
    DrawMenu( );

    // Note - we don't allow returning like normal states
    return UIState_Count;
}

void StateLoading::DrawMenu( )
{
    UIRenderer::DrawSprite( gMainBGHeader, 0, 0, BG_HEADER_WIDTH, BG_HEADER_HEIGHT );
    
    char loadStr[ MAX_PATH ] = { 0 };
    snprintf( loadStr, sizeof( loadStr ), "LOADING GAME" );
    int loadStrLen = strlen( loadStr );
    
    char gameStr[ MAX_PATH ] = { 0 };
    snprintf( gameStr, sizeof( gameStr ), "%s", ShockRomLoader::GetRomsetName( ) );
    int gameStrLen = strlen( gameStr );
    
    if( mLoadingTimerMS < gGlobalTimer.GetElapsedTimeMicroseconds() )
    {
        mLoadingTimerMS = gGlobalTimer.GetElapsedTimeMicroseconds() + 50 * MILLI_TO_MICROSECONDS;
        mLoadingColorLetterIndex = (mLoadingColorLetterIndex + 1) % (loadStrLen + gameStrLen);
    }
    
    int whiteColor = 0xFFFF;
    int loadColor  = UI_COLOR_ENABLED;
    int startYPos  = UI_Y_POS_MENU + UI_ROW_HEIGHT;
    
    // render the LOADING GAME line
    int loadLength = Font::MeasureStringWidth( loadStr, FontType_Upheaval );
    int xPos = (PLATFORM_LCD_WIDTH - loadLength) / 2;
    
    for( int i = 0; i < loadStrLen; i++ )
    {
        int letterColor = whiteColor;
        if ( mLoadingColorLetterIndex < loadStrLen && mLoadingColorLetterIndex == i )
        {
            letterColor = loadColor;
        }
        
        char letterStr[ MAX_PATH ] = { 0 };
        letterStr[ 0 ] = loadStr[ i ];
        UIRenderer::DrawText( letterStr, 
                              xPos, 
                              startYPos,
                              letterColor,
                              FontType_Upheaval );
                              
        xPos += UPHEAVAL_FONT_LETTER_WIDTH + FONT_SPACING;
    }
    
    // render the game line
    int gameLength = Font::MeasureStringWidth( gameStr, FontType_Upheaval );
    xPos = (PLATFORM_LCD_WIDTH - gameLength) / 2;
    
    for( int i = 0; i < gameStrLen; i++ )
    {
        int letterColor = whiteColor;
        if ( mLoadingColorLetterIndex >= loadStrLen && mLoadingColorLetterIndex - loadStrLen == i )
        {
            letterColor = loadColor;
        }
        
        char letterStr[ MAX_PATH ] = { 0 };
        letterStr[ 0 ] = gameStr[ i ];
        UIRenderer::DrawText( letterStr, 
                              xPos, 
                              startYPos + UI_ROW_HEIGHT * 2,
                              letterColor,
                              FontType_Upheaval );
                              
        xPos += UPHEAVAL_FONT_LETTER_WIDTH + FONT_SPACING;
    }
}
