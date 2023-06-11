
// See License.md for license

#include "shock/core/ostimer.h"
#include "shock/ui/render/uirenderer.h"
#include "shock/ui/states/stategamestatebase.h"
#include "shock/util/util.h"
#include "shock/shockgame.h"
#include "shock/input/shockinput.h"

void StateGameStateBase::Create( )
{
    UIBaseState::Create( );

    mNumMenuItems = 0;

    // store the full width for easier rendering
    mMenuItemFullWidth = Font::MeasureStringWidth( "Slot 8" ) + THUMB_IMAGE_SPACING + STATE_THUMBNAIL_WIDTH;

    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU + STATE_THUMBNAIL_HEIGHT / 2;
    int i = 0;
    for ( i = 0; i < MAX_SAVE_STATES / 2; i++ )
    {
        char menuTitle[ MAX_PATH ] = { 0 };
        snprintf( menuTitle, sizeof( menuTitle ), "Slot %d", i + 1 );
        mMenuItemList[ mNumMenuItems++ ].Create( menuTitle, xPos, yPos, 0xFFFF );

        yPos += STATE_THUMBNAIL_HEIGHT + STATE_THUMBNAIL_HEIGHT / 2;

        if ( mNumMenuItems > MAX_MENU_ITEMS )
        {
            flushPrintf( "StateGameStateBase::Create() ERROR!!! mNumMenuItems too large at %d."
                "MAX_MENU_ITEMS is only: %d\r\n",
                mNumMenuItems,
                MAX_MENU_ITEMS );
            exit( 0 );
        }
    }

    xPos = UI_WIDTH - mMenuItemFullWidth - UI_X_POS_MENU;
    yPos = UI_Y_POS_MENU + STATE_THUMBNAIL_HEIGHT / 2;
    for ( ; i < MAX_SAVE_STATES; i++ )
    {
        char menuTitle[ MAX_PATH ] = { 0 };
        snprintf( menuTitle, sizeof( menuTitle ), "Slot %d", i + 1 );
        mMenuItemList[ mNumMenuItems++ ].Create( menuTitle, xPos, yPos, 0xFFFF );

        yPos += STATE_THUMBNAIL_HEIGHT + STATE_THUMBNAIL_HEIGHT / 2;

        if ( mNumMenuItems > MAX_MENU_ITEMS )
        {
            flushPrintf( "StateGameStateBase::Create() ERROR!!! mNumMenuItems too large at %d."
                "MAX_MENU_ITEMS is only: %d\r\n",
                mNumMenuItems,
                MAX_MENU_ITEMS );
            exit( 0 );
        }
    }

    memset( mStateBlankImg, 0, sizeof( mStateBlankImg ) );
    memset( mResultStr, 0, sizeof( mResultStr ) );
    memset( mStateThumb, 0, sizeof( mStateThumb ) );
    memset( mStateExists, 0, sizeof( mStateExists ) );

    mMenuSelection       = 0;
    mSaveLoadThreadState = SaveLoadThreadState_None;
    mSaveLoadResult      = 0;

    mHeaderColorLetterIndex = 0;
    mAnimationTimerMS = 0;
}

void StateGameStateBase::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateGameStateBase::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );

    // reset our output
    memset( mResultStr, 0, sizeof( mResultStr ) );

    // see which state slots exist
    for ( int i = 0; i < MAX_SAVE_STATES; i++ )
    {
        int result = ShockGame::LoadGameStateThumbnail( i, mStateThumb[ i ] );
        mStateExists[ i ] = result > -1 ? 1 : 0;
    }

    mSaveLoadThreadState = SaveLoadThreadState_None;
    mSaveLoadResult      = 0;

    mHeaderColorLetterIndex = 0;
    mAnimationTimerMS = 0;
}

void StateGameStateBase::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
}

UIState StateGameStateBase::Update( )
{
    UIBaseState::Update( );

    if ( mSaveLoadThreadState == SaveLoadThreadState_None )
    {
        // check for menu navigation
        if ( ShockInput::GetInput( P1_Joy_Down )->WasReleased( ) )
        {
            mMenuSelection = ( mMenuSelection + 1 ) % mNumMenuItems;
        }
        else if ( ShockInput::GetInput( P1_Joy_Up )->WasReleased( ) )
        {
            mMenuSelection--;
            if ( mMenuSelection < 0 )
            {
                mMenuSelection = mNumMenuItems - 1;
            }
        }
        else if ( ShockInput::GetInput( P1_Joy_Left )->WasReleased( ) )
        {
            if ( mMenuSelection >= MAX_SAVE_STATES / 2 )
            {
                mMenuSelection -= MAX_SAVE_STATES / 2;
            }
        }
        else if ( ShockInput::GetInput( P1_Joy_Right )->WasReleased( ) )
        {
            if ( mMenuSelection < MAX_SAVE_STATES / 2 )
            {
                mMenuSelection += MAX_SAVE_STATES / 2;
            }
        }

        // should we exit?
        return UIBaseState::HandleBackButton( );
    }
    else
    {
        return UIState_Count;
    }
}

void StateGameStateBase::DrawMenu( const char *pHeader )
{
    // Header
    if ( mSaveLoadThreadState == SaveLoadThreadState_Running )
    {
        int headerLen = strlen( pHeader );
        if ( mAnimationTimerMS < gGlobalTimer.GetElapsedTimeMicroseconds( ) )
        {
            mAnimationTimerMS = gGlobalTimer.GetElapsedTimeMicroseconds( ) + 50 * MILLI_TO_MICROSECONDS;
            mHeaderColorLetterIndex = ( mHeaderColorLetterIndex + 1 ) % headerLen;
        }

        int whiteColor = 0xFFFF;
        int hilightColor = UI_COLOR_ENABLED;

        int headerLength = Font::MeasureStringWidth( pHeader, FontType_Upheaval );
        int xPos = ( UI_WIDTH - headerLength ) / 2;

        for ( int i = 0; i < headerLen; i++ )
        {
            int letterColor = whiteColor;
            if ( mHeaderColorLetterIndex < headerLen && mHeaderColorLetterIndex == i )
            {
                letterColor = hilightColor;
            }

            char letterStr[ MAX_PATH ] = { 0 };
            letterStr[ 0 ] = pHeader[ i ];
            UIRenderer::DrawText( letterStr,
                xPos,
                UI_Y_POS_HEADER_TITLE,
                letterColor,
                FontType_Upheaval );

            xPos += UPHEAVAL_FONT_LETTER_WIDTH + FONT_SPACING;
        }
    }
    else
    {
        UIBaseState::RenderTitle( pHeader );
    }

    // Render List of states
    int titleWidth = Font::MeasureStringWidth( mMenuItemList[ 0 ].GetText( ) );

    for ( int i = 0; i < MAX_SAVE_STATES; i++ )
    {
        if ( mStateExists[ i ] )
        {
            UIRenderer::DrawSprite( mStateThumb[ i ],
                mMenuItemList[ i ].GetXPos( ) + titleWidth + THUMB_IMAGE_SPACING,
                mMenuItemList[ i ].GetYPos( ) - STATE_THUMBNAIL_HEIGHT / 2,
                STATE_THUMBNAIL_WIDTH,
                STATE_THUMBNAIL_HEIGHT );
            mMenuItemList[ i ].SetColor( UI_COLOR_ENABLED );
        }
        else
        {
            UIRenderer::DrawSprite( mStateBlankImg,
                mMenuItemList[ i ].GetXPos( ) + titleWidth + THUMB_IMAGE_SPACING,
                mMenuItemList[ i ].GetYPos( ) - STATE_THUMBNAIL_HEIGHT / 2,
                STATE_THUMBNAIL_WIDTH,
                STATE_THUMBNAIL_HEIGHT );
            mMenuItemList[ i ].SetColor( 0xFFFF );
        }

        mMenuItemList[ i ].Draw( );
    }

    // Result
    if ( mResultStr[ 0 ] != 0 )
    {
        int resXPos = UIBaseState::GetCenteredXPos( mResultStr );
        UIRenderer::DrawText( mResultStr, resXPos, UI_HEIGHT - 100, 0xFFFF );
    }

    // Cursor
    UIBaseState::RenderMenuCursor( mMenuItemList[ mMenuSelection ].GetXPos( ),
        mMenuItemList[ mMenuSelection ].GetYPos( ) );

    UIBaseState::RenderBackOption( "Return" );
}

void StateGameStateBase::OnSaveLoadComplete( int result )
{
    mSaveLoadResult      = result;
    mSaveLoadThreadState = SaveLoadThreadState_Complete;
}
