
// See License.md for license

#ifndef STATEMAINMENU_H_
#define STATEMAINMENU_H_

#include "shock/ui/render/menuitem.h"
#include "shock/ui/states/uibasestate.h"

struct MenuItemUIState
{
    MenuItem menuItem;
    UIState gameState;
};

class StateMainMenu : public UIBaseState
{
public:
    virtual void    Create( );
    virtual void    Destroy( );
    
    virtual void    EnterState( UIState oldState );
    virtual void    ExitState( UIState newState );
    
    virtual UIState Update( );
    
            int     ShouldExitEmulator( );
            int     ShouldExitUI( );
    
private:
            void              DrawMenu( );

private:
            int               mMenuSelection;
            int               mNumMenuItems;
            MenuItemUIState   mMenuItemGameState[ MAX_MENU_ITEMS ];
            
            int               mExitEmulator;
            int               mExitUI;
    
};

#endif