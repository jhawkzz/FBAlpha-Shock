
// See License.md for license

#ifndef SHOCKPLAYERINPUT_H_
#define SHOCKPLAYERINPUT_H_

#include "shock/core/input.h"

struct ShockButtonToBurnInput
{
    ShockButton joyLookup[ GameInp_Joy_Count ];
    ShockButton fireButtonLookup[ GameInp_Fire_Count ];
    ShockButton coinLookup;
    ShockButton startLookup;
};

class ShockPlayerInput
{
public:
    static void Create( INT32 hardwareCode );
    static void SetGameDefaults( );
    
    static void LoadFireInputs( const char *pRomsetName );
    static void SaveFireInputs( const char *pRomsetName );
    
    static void Update( );
    
    static ShockButtonToBurnInput *GetInputMapForPlayer( int index );
    
private:
    static void SetGameDefaults_SNKNeoGeo( );
    static void SetGameDefaults_Capcom( );

private:
    static ShockButtonToBurnInput mShockButtonToBurnInput[ MAX_SHOCK_PLAYERS ];
    static int                    mHardwareCode;
};

#endif
