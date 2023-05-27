
// See License.md for license

#ifndef STATEBUTTONCONFIG_H_
#define STATEBUTTONCONFIG_H_

#define BUTTON_WIDTH  (50)
#define BUTTON_HEIGHT (55)

#include "shock/input/shockplayerinput.h"
#include "shock/ui/states/uibasestate.h"
#include "shock/ui/render/menuitem.h"

class StateButtonConfig : public UIBaseState
{
public:
    virtual void Create( );
    virtual void Destroy( );
    
    virtual void EnterState( UIState oldState );
    virtual void ExitState( UIState newState );
    
    virtual UIState Update( );
    
private:
    void      DrawMenu( );
    int       GetPrevButtonInput( ShockButton buttonIndex );
    int       GetNextButtonInput( ShockButton buttonIndex );

private:
    int       mConfiguringButton;
    MenuItem *mpSelectedItem;
    int       mNumButtonsPerPlayer[ MAX_SHOCK_PLAYERS ];
    MenuItem  mButtonInputList[ MAX_SHOCK_PLAYERS ][ GameInp_Fire_Count ];
    UINT16   *mpButtonImageMap[ ShockButton_Count ];
    MenuItem  mRestoreDefaults;
    int       mPlayerSelection;
    int       mButtonSelection;
    int       mButtonConfigAvailable;
};

#endif
