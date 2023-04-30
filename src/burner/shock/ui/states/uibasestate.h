
// See License.md for license

#ifndef UIBASESTATE_H_
#define UIBASESTATE_H_

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
    UIState_Count
};

#define UI_Y_POS_MENU      (200) //The start y position for menu items
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
    void    RenderBackOption( );

protected:
    UIState   mLastState;
    int       mDrawBGImage;
};

#endif
