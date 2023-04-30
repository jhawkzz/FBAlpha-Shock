
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
    if( mLoadingTimerMS < gGlobalTimer.GetElapsedTimeMicroseconds() )
    {
        mLoadingTimerMS = gGlobalTimer.GetElapsedTimeMicroseconds() + 100 * MILLI_TO_MICROSECONDS;
        mLoadingColorLetterIndex = (mLoadingColorLetterIndex + 1) % 7;
    }
        
    int whiteColor = 0xFFFF;
    int loadColor  = UI_COLOR_ENABLED;
    
    int loadLength = Font::MeasureStringWidth( "LOADING" );
    int xPos = (PLATFORM_LCD_WIDTH - loadLength) / 2;
    UIRenderer::DrawText( "L", 
                        xPos, 
                        UI_Y_POS_MENU,
                        mLoadingColorLetterIndex == 0 ? loadColor : whiteColor );
    
    xPos += FONT_LETTER_FULL_WIDTH;
    UIRenderer::DrawText( "O", 
                        xPos, 
                        UI_Y_POS_MENU,
                        mLoadingColorLetterIndex == 1 ? loadColor : whiteColor );
                        
    xPos += FONT_LETTER_FULL_WIDTH;                    
    UIRenderer::DrawText( "A", 
                        xPos, 
                        UI_Y_POS_MENU,
                        mLoadingColorLetterIndex == 2 ? loadColor : whiteColor );
                        
    xPos += FONT_LETTER_FULL_WIDTH;
    UIRenderer::DrawText( "D", 
                        xPos, 
                        UI_Y_POS_MENU,
                        mLoadingColorLetterIndex == 3 ? loadColor : whiteColor );
                        
    xPos += FONT_LETTER_FULL_WIDTH;
    UIRenderer::DrawText( "I", 
                        xPos, 
                        UI_Y_POS_MENU,
                        mLoadingColorLetterIndex == 4 ? loadColor : whiteColor );
                        
    xPos += FONT_LETTER_FULL_WIDTH;
    UIRenderer::DrawText( "N", 
                        xPos, 
                        UI_Y_POS_MENU,
                        mLoadingColorLetterIndex == 5 ? loadColor : whiteColor );
                        
    xPos += FONT_LETTER_FULL_WIDTH;
    UIRenderer::DrawText( "G", 
                        xPos, 
                        UI_Y_POS_MENU,
                        mLoadingColorLetterIndex == 6 ? loadColor : whiteColor );
}
