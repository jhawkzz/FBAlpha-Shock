
// See License.md for license

#ifndef SHOCKPLAYERINPUT_H_
#define SHOCKPLAYERINPUT_H_

struct OSInputToBurnInput
{
    InputCodeToButtonMapping osInputToJoyLookup[ GameInp_Joy_Count ];
    InputCodeToButtonMapping osInputToFireButtonLookup[ GameInp_Fire_Count ];
    InputCodeToButtonMapping osInputToCoinLookup;
    InputCodeToButtonMapping osInputToStartLookup;
};

class ShockPlayerInput
{
public:
    static void Create( INT32 hardwareCode );
    static void SetGameDefaults( );
    
    static void LoadFireInputs( const char *pRomsetName );
    static void SaveFireInputs( const char *pRomsetName );
    
    static void Update( );
    
    static OSInputToBurnInput *GetInputMapForPlayer( int index );
    
private:
    static void SetGameDefaults_SNKNeoGeo( );
    static void SetGameDefaults_Capcom( );

private:
    static OSInputToBurnInput mOSInputToBurnInput[ MAX_MVSX_PLAYERS ];
    static int                mHardwareCode;
};

#endif
