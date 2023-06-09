
// See License.md for license

#ifndef STATEEMULATORSETTINGS_H_
#define STATEEMULATORSETTINGS_H_

#include "shock/ui/render/menuitem.h"
#include "shock/ui/states/uibasestate.h"

class StateEmulatorSettings : public UIBaseState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );
    
    virtual UIState Update( );
    
private:
    void      DrawMenu( );
    
    int       mNumMenuItems;
    MenuItem  mMenuItemList[ MAX_MENU_ITEMS ];
    int       mMenuSelection;

    MenuItem* mFpsMenu;
    MenuItem* mTimerMenu;
    MenuItem* mDisplayMenu;
};

#endif
