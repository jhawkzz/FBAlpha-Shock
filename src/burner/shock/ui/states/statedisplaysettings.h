
// See License.md for license

#ifndef STATEDISPLAYSETTINGS_H_
#define STATEDISPLAYSETTINGS_H_

#include "shock/ui/render/menuitem.h"
#include "shock/ui/states/uibasestate.h"

class StateDisplaySettings : public UIBaseState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );
    
    virtual UIState Update( );
    
private:
    void      DrawMenu( );
    
    int       mEnabledSettings;  
    
    int       mNumMenuItems;
    MenuItem  mMenuItemList[ MAX_MENU_ITEMS ];
    int       mMenuSelection;
    char      mResultStr[ MAX_PATH ];
};

#endif
