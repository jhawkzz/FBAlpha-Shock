
// See License.md for license

#ifndef STATEEMULATORSETTINGS_H_
#define STATEEMULATORSETTINGS_H_

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
    char      mResultStr[ MAX_PATH ];
};

#endif
