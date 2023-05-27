
// See License.md for license

#ifndef SHOCKBURNINPUT_H_
#define SHOCKBURNINPUT_H_

#include "shock/systems.h"

struct BurnDIPInfo;
struct BurnInputInfo;

// Trying to think of a game with more than _NINE_ players and _NINE_ fire buttons. 
// X-Men arcade has 6 and THAT'S a lot.
#define MAX_NUM_PLAYERS (9)

#define MAX_BURN_INPUTS (4096)

enum GameInp_Joy
{
    GameInp_Joy_Up,
    GameInp_Joy_Right,
    GameInp_Joy_Down,
    GameInp_Joy_Left,
    GameInp_Joy_Count
};

enum GameInp_Fire
{
    GameInp_Fire_0,
    GameInp_Fire_1,
    GameInp_Fire_2,
    GameInp_Fire_3,
    GameInp_Fire_4,
    GameInp_Fire_5,
    GameInp_Fire_Count
};

struct GameInp 
{
	UINT8 nType;
	union 
    {
		UINT8  *pVal;      // Most inputs use a char*
		UINT16 *pShortVal; // All analog inputs use a short*
	};
};

struct GameInpWrapper
{
    GameInp *pGameInp;
    char     inputDisplayName[ MAX_PATH ];
    char     inputInfoName[ MAX_PATH ];
};

struct PlayerInp
{
    GameInpWrapper joyInput[ GameInp_Joy_Count ];
    
    GameInpWrapper fireInput[ GameInp_Fire_Count ];
    
    GameInpWrapper startButton;
    GameInpWrapper coinButton;
};

class ShockBurnInput
{
    // these classes are peers--BurnInput handles
    // exposing the game inputs, PlayerInput handles exposing
    // the Hardware Inputs
    friend class ShockPlayerInput;
public:
    static int  Create( );
    
    static void ToggleDiagnosticMode( int diagIndex, UINT8 val );
    static void ToggleReset( UINT8 val );
    
    static int  GameHasDiagnosticMode( );
    static int  GameHasReset( );
    
    static int        GetNumPlayers( );
    static PlayerInp *GetPlayerInput( int index );
    
private:
    static int  GameInpInit( );
    
    static void SetPlayerInput( int playerIndex, GameInp *pGameInpInfo, BurnInputInfo *pBurnInputInfo );
    static void AssignInputWrapper( GameInpWrapper *pInpWrapper, GameInp *pGameInpInfo, BurnInputInfo *pBurnInputInfo );
    
    static void InpDIPSWGetOffset( );
    static void InpDIPSWResetDIPs( );
    
    static void PrintDIPInfo( BurnDIPInfo *pDipInfo );
    static void PrintInputInfo( BurnInputInfo *pInputInfo );
    
private:
    static int            mNumTotalGameInputs;
    static GameInp        mGameInputList[ MAX_BURN_INPUTS ];
    
    static int            mDIPOffset;
    static int            mNumDipInputs;
    static GameInpWrapper mDipInputList[ MAX_BURN_INPUTS ];
    
    static int            mNumPlayers;
    static PlayerInp      mPlayerInputList[ MAX_NUM_PLAYERS ];
    
    static GameInpWrapper mDiagnosticInput;
    static GameInpWrapper mResetInput;
    static GameInpWrapper mServiceInput;
};

#endif
