
// See License.md for license

#ifndef STATEBUTTONCONFIG_H_
#define STATEBUTTONCONFIG_H_

#define BUTTON_WIDTH  (50)
#define BUTTON_HEIGHT (55)

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
<<<<<<< HEAD
    int       mNumButtonsPerPlayer[ MAX_SHOCK_PLAYERS ];
    MenuItem  mButtonInputList[ MAX_SHOCK_PLAYERS ][ GameInp_Fire_Count ];
    UINT16   *mpButtonImageMap[ ShockButton_Count ];
=======
    int       mNumButtonsPerPlayer[ MAX_MVSX_PLAYERS ];
    MenuItem  mButtonInputList[ MAX_MVSX_PLAYERS ][ GameInp_Fire_Count ];
    UINT16   *mpButtonImageMap[ Button_Count ];
>>>>>>> e8a4db5 (wip)
    MenuItem  mRestoreDefaults;
    int       mPlayerSelection;
    int       mButtonSelection;
    int       mButtonConfigAvailable;
};

#endif
