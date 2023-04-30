
// See License.md for license

#ifndef STATEGAMESETTINGS_H_
#define STATEGAMESETTINGS_H_


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
    
    MenuItem  mMenuItemList[ MAX_MENU_ITEMS ];
    int       mNumMenuItems;
    int       mMenuSelection;
};

#endif
