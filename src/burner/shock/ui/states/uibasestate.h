
// See License.md for license

#ifndef UIBASESTATE_H_
#define UIBASESTATE_H_

#include "shock/shock.h"
#include "shock/font/font.h"

enum UIState
{
    UIState_MainMenu,
    UIState_LoadState,
    UIState_SaveState,
    UIState_GameSettings,
    UIState_ButtonConfig,
    UIState_DisplaySettings,
    UIState_EmulatorSettings,
    UIState_Credits,
    UIState_Loading,
    UIState_LoadError,
    UIState_Count
};


#define BG_HEADER_WIDTH  (1280)
#define BG_HEADER_HEIGHT (179)

#define UI_Y_POS_HEADER_TITLE     (75)    // The Y position where the header title should render
#define UI_Y_POS_HEADER_SEPERATOR (162) // The Y position of the line seperating the header
#define UI_Y_POS_MENU      (181) //The start y position for menu items
#define UI_X_POS_MENU      (90)  //The start x position for menu items
#define UI_ROW_HEIGHT      (56)  //The amount to increment Y by when rendering another line
#define UI_CURSOR_X_OFFSET (52)  //The amount of pixels to decrement by when rendering the selection cursor
#define MAX_MENU_ITEMS     (15)
#define UI_COLOR_DISABLED  (0x4A49)
#define UI_COLOR_ENABLED   (0XFFEA)

class UIBaseState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );
    
    virtual UIState Update( );
    
protected:
    UIState HandleBackButton( );
    void    RenderBackOption( const char *pNavVerb );
    void    RenderMenuCursor( int menuX, int menuY );
    void    RenderTitle( const char *pText );
    
    int     GetCenteredXPos( const char *pText, FontType fontType = FontType_Met );

protected:
    UIState mLastState;
};

#endif
