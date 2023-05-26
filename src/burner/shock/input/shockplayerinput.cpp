
// See License.md for license

#include "shock/includes.h"
#include "shock/input/shockinput.h"

#ifdef MVSX
   #include "shock/platform/mvsxled.h"
#endif

ShockButtonToBurnInput ShockPlayerInput::mShockButtonToBurnInput[ MAX_SHOCK_PLAYERS ];
int                    ShockPlayerInput::mHardwareCode;

void ShockPlayerInput::Create( INT32 hardwareCode )
{
    // Set default values that are not allowed to change
    if( MAX_SHOCK_PLAYERS < 2 )
    {
        flushPrintf( "ERROR! MAX_SHOCK_PLAYERS MUST BE AT LEAST 2. Found: %d", MAX_SHOCK_PLAYERS );
        exit( 0 );
    }
    
    // Player 1
    mShockButtonToBurnInput[ 0 ].coinLookup  = P1_InsertCoin;
    mShockButtonToBurnInput[ 0 ].startLookup = P1_Start;
    
    mShockButtonToBurnInput[ 0 ].joyLookup[ GameInp_Joy_Up ]    = P1_Joy_Up;
    mShockButtonToBurnInput[ 0 ].joyLookup[ GameInp_Joy_Right ] = P1_Joy_Right;
    mShockButtonToBurnInput[ 0 ].joyLookup[ GameInp_Joy_Down ]  = P1_Joy_Down;
    mShockButtonToBurnInput[ 0 ].joyLookup[ GameInp_Joy_Left ]  = P1_Joy_Left;
    
    // Player 2
    mShockButtonToBurnInput[ 1 ].coinLookup  = P2_InsertCoin;
    mShockButtonToBurnInput[ 1 ].startLookup = P2_Start;
    
    mShockButtonToBurnInput[ 1 ].joyLookup[ GameInp_Joy_Up ]    = P2_Joy_Up;
    mShockButtonToBurnInput[ 1 ].joyLookup[ GameInp_Joy_Right ] = P2_Joy_Right;
    mShockButtonToBurnInput[ 1 ].joyLookup[ GameInp_Joy_Down ]  = P2_Joy_Down;
    mShockButtonToBurnInput[ 1 ].joyLookup[ GameInp_Joy_Left ]  = P2_Joy_Left;
    
    mHardwareCode = hardwareCode;
    
    // the hardware code provides a hint for how to setup defaults for the buttons.
    // (e.g. Street Fighter is 1-2-3
    //                         4-5-6
    // (but Neo Geo is 2-3-4
    //                 1-5-6)
    SetGameDefaults( );
}

void ShockPlayerInput::SetGameDefaults( )
{
    //flushPrintf( "HardwareCode: 0x%08x, Masked: 0x%08x\r\n", hardwareCode, (hardwareCode & HARDWARE_PUBLIC_MASK) );
    
    // JHM: Note - Burn SAYS:
    // (From Burn.h line 318)Format: 0xDDEEFFFF, where EE: Manufacturer, DD: Hardware platform, FFFF: Flags (used by driver)
    // However, first, I find that to be backwards based directly on the defines below it.
    // (From Burn.h line 329) #define HARDWARE_PREFIX_SNK (0x05000000) [Note the manu is in the first, but their mask example
    // says it should be the 0x00XX0000]
    // 
    // Secondly, for Neo Geo games only, they're actually using the highest 4 bits for a Cartridge flag.
    // So its like they intended to use the highest 8 bits as the hardware, but mixed it around.
    // Anyways, this works.
    if( (mHardwareCode & HARDWARE_PUBLIC_MASK) == HARDWARE_SNK_NEOGEO
     || (mHardwareCode & HARDWARE_PUBLIC_MASK) == (HARDWARE_PREFIX_CARTRIDGE | HARDWARE_SNK_NEOGEO) )
    {
        //flushPrintf( "Neo Geo Game Detected. Setting default inputs to that.\r\n" );
        SetGameDefaults_SNKNeoGeo( );
    }
    else
    {
        // For anything not specifically checked, use Capcom defaults--they work really well.
        //flushPrintf( "Misc Vendor Detected. Setting default inputs to Capcom.\r\n" );
        SetGameDefaults_Capcom( );
    }
}

void ShockPlayerInput::LoadFireInputs( const char *pRomsetName )
{
    SavedFireInput *pFireInput = ShockConfig::LoadFireInputs( pRomsetName );
    if( pFireInput != NULL )
    {
        for( int i = 0; i < MAX_SHOCK_PLAYERS; i++ )
        {
            memcpy( mShockButtonToBurnInput[ i ].fireButtonLookup,
                    pFireInput->fireButtonLookup[ i ],
                    sizeof( mShockButtonToBurnInput[ i ].fireButtonLookup ) );
        }
    }
    else
    {
        flushPrintf( "Warning - Could not load inputs for game %s\r\n", pRomsetName );
    }
}

void ShockPlayerInput::SaveFireInputs( const char *pRomsetName )
{
    SavedFireInput fireInput = { 0 };
        
    // copy the values out
    for( int i = 0; i < MAX_SHOCK_PLAYERS; i++ )
    {
        memcpy( fireInput.fireButtonLookup[ i ],
                mShockButtonToBurnInput[ i ].fireButtonLookup, 
                sizeof( fireInput.fireButtonLookup[ i ] ) );
    }
    
    ShockConfig::SaveFireInputs( pRomsetName, &fireInput );
}

void ShockPlayerInput::Update( )
{
    ShockInput::Update( );
    
    for( int i = 0; i < min( MAX_SHOCK_PLAYERS, ShockBurnInput::mNumPlayers ); i++ )
    {
        // Coin and Start
        if( ShockBurnInput::mPlayerInputList[ i ].coinButton.pGameInp != NULL )
        {
            *ShockBurnInput::mPlayerInputList[ i ].coinButton.pGameInp->pVal = 
                        (UINT8)ShockInput::GetInput( mShockButtonToBurnInput[ i ].coinLookup )->GetState();
                       
#ifdef MVSX
            if( (UINT16)ShockInput::GetInput( mShockButtonToBurnInput[ i ].coinLookup )->WasPressed() )
            {
                int playerLed = MVSXLed::GetLED( i );
                playerLed++;
                MVSXLed::SetLED( i, playerLed );
            }
#endif
        }
        
        if( ShockBurnInput::mPlayerInputList[ i ].startButton.pGameInp != NULL )
        {
            *ShockBurnInput::mPlayerInputList[ i ].startButton.pGameInp->pVal = 
                        (UINT8)ShockInput::GetInput( mShockButtonToBurnInput[ i ].startLookup )->GetState( );
                        
#ifdef MVSX
            if( (UINT16)ShockInput::GetInput( mShockButtonToBurnInput[ i ].startLookup )->WasReleased() )
            {
                int playerLed = MVSXLed::GetLED( i );
                playerLed--;
                MVSXLed::SetLED( i, playerLed );
            }
#endif
        }
        
        // Joystick
        for( int c = 0; c < GameInp_Joy_Count; c++ )
        {
            if( ShockBurnInput::mPlayerInputList[ i ].joyInput[ c ].pGameInp != NULL )
            {
                *ShockBurnInput::mPlayerInputList[ i ].joyInput[ c ].pGameInp->pVal = 
                        (UINT16)ShockInput::GetInput( mShockButtonToBurnInput[ i ].joyLookup[ c ] )->GetState();
            }
        }
    
        for( int k = 0; k < GameInp_Fire_Count; k++ )
        {
            // make sure this game HAS this button
            if( ShockBurnInput::mPlayerInputList[ i ].fireInput[ k ].pGameInp != NULL )
            {
                *ShockBurnInput::mPlayerInputList[ i ].fireInput[ k ].pGameInp->pVal = 
                        (UINT16)ShockInput::GetInput( mShockButtonToBurnInput[ i ].fireButtonLookup[ k ] )->GetState();
            }   
        }
    }
}

ShockButtonToBurnInput *ShockPlayerInput::GetInputMapForPlayer( int index )
{
    return &mShockButtonToBurnInput[ index ];
}

void ShockPlayerInput::SetGameDefaults_SNKNeoGeo( )
{
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 0 ] = P1_Button_1;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 1 ] = P1_Button_2;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 2 ] = P1_Button_3;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 3 ] = P1_Button_4;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 4 ] = P1_Button_5;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 5 ] = P1_Button_6;
    
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 0 ] = P2_Button_1;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 1 ] = P2_Button_2;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 2 ] = P2_Button_3;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 3 ] = P2_Button_4;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 4 ] = P2_Button_5;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 5 ] = P2_Button_6;
}

void ShockPlayerInput::SetGameDefaults_Capcom( )
{
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 0 ] = P1_Button_2;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 1 ] = P1_Button_3;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 2 ] = P1_Button_4;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 3 ] = P1_Button_1;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 4 ] = P1_Button_5;
    mShockButtonToBurnInput[ 0 ].fireButtonLookup[ 5 ] = P1_Button_6;
    
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 0 ] = P2_Button_2;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 1 ] = P2_Button_3;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 2 ] = P2_Button_4;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 3 ] = P2_Button_1;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 4 ] = P2_Button_5;
    mShockButtonToBurnInput[ 1 ].fireButtonLookup[ 5 ] = P2_Button_6;
}
