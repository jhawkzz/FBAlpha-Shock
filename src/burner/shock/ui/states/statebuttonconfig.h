
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
    int       GetPrevButtonInput( InputCodeToButtonMapping buttonIndex );
    int       GetNextButtonInput( InputCodeToButtonMapping buttonIndex );

private:
    int       mConfiguringButton;
    MenuItem *mpSelectedItem;
    int       mNumButtonsPerPlayer[ MAX_MVSX_PLAYERS ];
    MenuItem  mButtonInputList[ MAX_MVSX_PLAYERS ][ GameInp_Fire_Count ];
    UINT16   *mpButtonImageMap[ Button_Count ];
    MenuItem  mRestoreDefaults;
    int       mPlayerSelection;
    int       mButtonSelection;
    char      mResultStr[ MAX_PATH ];
};

#endif
