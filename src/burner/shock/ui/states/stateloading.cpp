
// See License.md for license

#include "../../includes.h"

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
    char loadStr[ MAX_PATH ] = { 0 };
    snprintf( loadStr, sizeof( loadStr ), "LOADING GAME: %s", ShockRomLoader::GetRomsetName( ) );
    int loadStrLen = strlen( loadStr );
    
    if( mLoadingTimerMS < gGlobalTimer.GetElapsedTimeMicroseconds() )
    {
        mLoadingTimerMS = gGlobalTimer.GetElapsedTimeMicroseconds() + 100 * MILLI_TO_MICROSECONDS;
        mLoadingColorLetterIndex = (mLoadingColorLetterIndex + 1) % loadStrLen;
    }
        
    int whiteColor = 0xFFFF;
    int loadColor  = UI_COLOR_ENABLED;
    
    int loadLength = Font::MeasureStringWidth( loadStr );
    int xPos = (PLATFORM_LCD_WIDTH - loadLength) / 2;
    
    for( int i = 0; i < loadStrLen; i++ )
    {
        char letterStr[ MAX_PATH ] = { 0 };
        letterStr[ 0 ] = loadStr[ i ];
        UIRenderer::DrawText( letterStr, 
                              xPos, 
                              UI_Y_POS_MENU,
                              mLoadingColorLetterIndex == i ? loadColor : whiteColor );
                              
        xPos += FONT_LETTER_FULL_WIDTH;
    }
}
