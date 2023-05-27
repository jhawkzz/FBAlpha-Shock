
// See License.md for license

#ifndef STATEGAMESETTINGS_H_
#define STATEGAMESETTINGS_H_

#include "shock/ui/states/uibasestate.h"
#include "shock/ui/render/menuitem.h"

class StateGameSettings : public UIBaseState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );
    
    virtual UIState Update( );
    
private:
    void      DrawMenu( );

private:
    MenuItem  mMenuItemList[ MAX_MENU_ITEMS ];
    int       mNumMenuItems;
    int       mMenuSelection;
};

#endif
