
// See License.md for license

#include "../includes.h"

OSInputToBurnInput ShockPlayerInput::mOSInputToBurnInput[ MAX_SHOCK_PLAYERS ];
int                ShockPlayerInput::mHardwareCode;

void ShockPlayerInput::Create( INT32 hardwareCode )
{
    // Set default values that are not allowed to change
    if( MAX_SHOCK_PLAYERS < 2 )
    {
        flushPrintf( "ERROR! MAX_SHOCK_PLAYERS MUST BE AT LEAST 2. Found: %d", MAX_SHOCK_PLAYERS );
        exit( 0 );
    }
    
    // Player 1
    mOSInputToBurnInput[ 0 ].osInputToCoinLookup  = OptionsBack;
    mOSInputToBurnInput[ 0 ].osInputToStartLookup = P1_Start;
    
    mOSInputToBurnInput[ 0 ].osInputToJoyLookup[ GameInp_Joy_Up ]    = P1_Joy_Up;
    mOSInputToBurnInput[ 0 ].osInputToJoyLookup[ GameInp_Joy_Right ] = P1_Joy_Right;
    mOSInputToBurnInput[ 0 ].osInputToJoyLookup[ GameInp_Joy_Down ]  = P1_Joy_Down;
    mOSInputToBurnInput[ 0 ].osInputToJoyLookup[ GameInp_Joy_Left ]  = P1_Joy_Left;
    
    // Player 2
    mOSInputToBurnInput[ 1 ].osInputToCoinLookup  = SelectGame;
    mOSInputToBurnInput[ 1 ].osInputToStartLookup = P2_Start;
    
    mOSInputToBurnInput[ 1 ].osInputToJoyLookup[ GameInp_Joy_Up ]    = P2_Joy_Up;
    mOSInputToBurnInput[ 1 ].osInputToJoyLookup[ GameInp_Joy_Right ] = P2_Joy_Right;
    mOSInputToBurnInput[ 1 ].osInputToJoyLookup[ GameInp_Joy_Down ]  = P2_Joy_Down;
    mOSInputToBurnInput[ 1 ].osInputToJoyLookup[ GameInp_Joy_Left ]  = P2_Joy_Left;
    
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
            memcpy( mOSInputToBurnInput[ i ].osInputToFireButtonLookup,
                    pFireInput->osInputToFireButtonLookup[ i ],
                    sizeof( mOSInputToBurnInput[ i ].osInputToFireButtonLookup ) );
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
        memcpy( fireInput.osInputToFireButtonLookup[ i ],
                mOSInputToBurnInput[ i ].osInputToFireButtonLookup, 
                sizeof( fireInput.osInputToFireButtonLookup[ i ] ) );
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
                        (UINT8)ShockInput::GetInput( mOSInputToBurnInput[ i ].osInputToCoinLookup )->GetState();
                       
#ifdef MVSX
            if( (UINT16)ShockInput::GetInput( mOSInputToBurnInput[ i ].osInputToCoinLookup )->WasPressed() )
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
                        (UINT8)ShockInput::GetInput( mOSInputToBurnInput[ i ].osInputToStartLookup )->GetState( );
                        
#ifdef MVSX
            if( (UINT16)ShockInput::GetInput( mOSInputToBurnInput[ i ].osInputToStartLookup )->WasReleased() )
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
                        (UINT16)ShockInput::GetInput( mOSInputToBurnInput[ i ].osInputToJoyLookup[ c ] )->GetState();
            }
        }
    
        for( int k = 0; k < GameInp_Fire_Count; k++ )
        {
            // make sure this game HAS this button
            if( ShockBurnInput::mPlayerInputList[ i ].fireInput[ k ].pGameInp != NULL )
            {
                *ShockBurnInput::mPlayerInputList[ i ].fireInput[ k ].pGameInp->pVal = 
                        (UINT16)ShockInput::GetInput( mOSInputToBurnInput[ i ].osInputToFireButtonLookup[ k ] )->GetState();
            }   
        }
    }
}

OSInputToBurnInput *ShockPlayerInput::GetInputMapForPlayer( int index )
{
    return &mOSInputToBurnInput[ index ];
}

void ShockPlayerInput::SetGameDefaults_SNKNeoGeo( )
{
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 0 ] = P1_Red;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 1 ] = P1_Yellow;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 2 ] = P1_Green;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 3 ] = P1_Blue;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 4 ] = P1_MidBot;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 5 ] = P1_RightBot;
    
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 0 ] = P2_Red;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 1 ] = P2_Yellow;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 2 ] = P2_Green;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 3 ] = P2_Blue;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 4 ] = P2_MidBot;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 5 ] = P2_RightBot;
}

void ShockPlayerInput::SetGameDefaults_Capcom( )
{
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 0 ] = P1_Yellow;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 1 ] = P1_Green;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 2 ] = P1_Blue;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 3 ] = P1_Red;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 4 ] = P1_MidBot;
    mOSInputToBurnInput[ 0 ].osInputToFireButtonLookup[ 5 ] = P1_RightBot;
    
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 0 ] = P2_Yellow;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 1 ] = P2_Green;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 2 ] = P2_Blue;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 3 ] = P2_Red;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 4 ] = P2_MidBot;
    mOSInputToBurnInput[ 1 ].osInputToFireButtonLookup[ 5 ] = P2_RightBot;
}
