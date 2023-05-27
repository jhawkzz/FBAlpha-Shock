
// See License.md for license

#include "../../includes.h"

void StateSaveState::Create( )
{
    UIBaseState::Create( );
    
    mNumMenuItems = 0;

    // store the full width for easier rendering
    mMenuItemFullWidth = Font::MeasureStringWidth("Slot 8") + THUMB_IMAGE_SPACING + STATE_THUMBNAIL_WIDTH;
    
    int xPos = UI_X_POS_MENU;
    int yPos = UI_Y_POS_MENU + STATE_THUMBNAIL_HEIGHT / 2;
    int i = 0;
    for (i = 0; i < MAX_SAVE_STATES / 2; i++)
    {
        char menuTitle[MAX_PATH] = { 0 };
        snprintf(menuTitle, sizeof(menuTitle), "Slot %d", i + 1);
        mMenuItemList[mNumMenuItems++].Create(menuTitle, xPos, yPos, 0xFFFF);

        yPos += STATE_THUMBNAIL_HEIGHT + STATE_THUMBNAIL_HEIGHT / 2;

        if (mNumMenuItems > MAX_MENU_ITEMS)
        {
            flushPrintf("StateSaveState::Create() ERROR!!! mNumMenuItems too large at %d."
                "MAX_MENU_ITEMS is only: %d\r\n",
                mNumMenuItems,
                MAX_MENU_ITEMS);
            exit(0);
        }
    }

    xPos = PLATFORM_LCD_WIDTH - mMenuItemFullWidth - UI_X_POS_MENU;
    yPos = UI_Y_POS_MENU + STATE_THUMBNAIL_HEIGHT / 2;
    for ( ; i < MAX_SAVE_STATES; i++)
    {
        char menuTitle[MAX_PATH] = { 0 };
        snprintf(menuTitle, sizeof(menuTitle), "Slot %d", i + 1);
        mMenuItemList[mNumMenuItems++].Create(menuTitle, xPos, yPos, 0xFFFF);

        yPos += STATE_THUMBNAIL_HEIGHT + STATE_THUMBNAIL_HEIGHT / 2;

        if (mNumMenuItems > MAX_MENU_ITEMS)
        {
            flushPrintf("StateSaveState::Create() ERROR!!! mNumMenuItems too large at %d."
                "MAX_MENU_ITEMS is only: %d\r\n",
                mNumMenuItems,
                MAX_MENU_ITEMS);
            exit(0);
        }
    }

    memset(mStateBlankImg, 0, sizeof(mStateBlankImg));
    memset(mResultStr, 0, sizeof( mResultStr ) );
    memset(mStateThumb, 0, sizeof(mStateThumb));
    memset(mStateExists, 0, sizeof(mStateExists));

    mMenuSelection = 0;
}

void StateSaveState::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateSaveState::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );
        
    // reset our output
    memset( mResultStr, 0, sizeof( mResultStr ) );

    // see which state slots exist
    for (int i = 0; i < MAX_SAVE_STATES; i++)
    {
        int result = ShockGame::LoadGameStateThumbnail(i, mStateThumb[i]);
        mStateExists[i] = result > -1 ? 1 : 0;
    }
}

void StateSaveState::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
}

UIState StateSaveState::Update( )
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
        int result = ShockGame::SaveGameState( mMenuSelection, mStateThumb[ mMenuSelection ]);
        if (result == -1 )
        {
            memset( mStateThumb[mMenuSelection], 0, sizeof( mStateThumb[ mMenuSelection ] ) );
            mStateExists[mMenuSelection] = 0;
            snprintf(mResultStr, sizeof( mResultStr ), "Error creating save state for slot: %d", mMenuSelection + 1 );
        }
        else
        {
            mStateExists[mMenuSelection] = 1;
            memset(mResultStr, 0, sizeof(mResultStr) );
        }
    }    
    
    // should we exit?
    return UIBaseState::HandleBackButton( );
}

void StateSaveState::DrawMenu( )
{
    UIBaseState::RenderTitle( "SAVE STATE" );
 
    int titleWidth = Font::MeasureStringWidth(mMenuItemList[0].GetText());

    for (int i = 0; i < MAX_SAVE_STATES; i++)
    {
        if (mStateExists[i])
        {
            UIRenderer::DrawSprite(mStateThumb[i],
                mMenuItemList[i].GetXPos() + titleWidth + THUMB_IMAGE_SPACING,
                mMenuItemList[i].GetYPos() - STATE_THUMBNAIL_HEIGHT / 2,
                STATE_THUMBNAIL_WIDTH,
                STATE_THUMBNAIL_HEIGHT);
            mMenuItemList[i].SetColor(UI_COLOR_ENABLED);
        }
        else
        {
            UIRenderer::DrawSprite(mStateBlankImg,
                mMenuItemList[i].GetXPos() + titleWidth + THUMB_IMAGE_SPACING,
                mMenuItemList[i].GetYPos() - STATE_THUMBNAIL_HEIGHT / 2,
                STATE_THUMBNAIL_WIDTH,
                STATE_THUMBNAIL_HEIGHT);
            mMenuItemList[i].SetColor(UI_COLOR_DISABLED);
        }

        mMenuItemList[ i ].Draw();
    }

    if (mResultStr[0] != 0)
    {
        int resXPos = UIBaseState::GetCenteredXPos(mResultStr);
        UIRenderer::DrawText(mResultStr, resXPos, PLATFORM_LCD_HEIGHT - 100, 0xFFFF);
    }

    // Cursor
    UIBaseState::RenderMenuCursor( mMenuItemList[ mMenuSelection ].GetXPos( ), 
                                   mMenuItemList[ mMenuSelection ].GetYPos( ) );
    
    UIBaseState::RenderBackOption( "Return" );
}
