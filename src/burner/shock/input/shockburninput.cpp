
// See License.md for license

#include "../includes.h"

int            ShockBurnInput::mNumTotalGameInputs;
GameInp        ShockBurnInput::mGameInputList[ MAX_BURN_INPUTS ];

int            ShockBurnInput::mDIPOffset;
int            ShockBurnInput::mNumDipInputs;
GameInpWrapper ShockBurnInput::mDipInputList[ MAX_BURN_INPUTS ];
    
int            ShockBurnInput::mNumPlayers;
PlayerInp      ShockBurnInput::mPlayerInputList[ MAX_NUM_PLAYERS ];

GameInpWrapper ShockBurnInput::mDiagnosticInput;
GameInpWrapper ShockBurnInput::mResetInput;
GameInpWrapper ShockBurnInput::mServiceInput;

int ShockBurnInput::Create( )
{
    GameInpInit( );
    
    InpDIPSWResetDIPs( );
}

void ShockBurnInput::ToggleDiagnosticMode( int diagIndex, UINT8 val )
{
    // todo: handle games with multiple diag buttons
    if( ShockBurnInput::mDiagnosticInput.pGameInp != NULL )
    {
        *ShockBurnInput::mDiagnosticInput.pGameInp->pVal = val;
    }
}

void ShockBurnInput::ToggleReset( UINT8 val )
{
    if ( ShockBurnInput::mResetInput.pGameInp != NULL )
    {
        *ShockBurnInput::mResetInput.pGameInp->pVal = val;
    }
}

int ShockBurnInput::GameHasDiagnosticMode( )
{
    return ShockBurnInput::mDiagnosticInput.pGameInp != NULL ? 1 : 0;
}

int ShockBurnInput::GameHasReset( )
{
    return ShockBurnInput::mResetInput.pGameInp != NULL ? 1 : 0;
}

int ShockBurnInput::GetNumPlayers( )
{
    return mNumPlayers;
}

PlayerInp *ShockBurnInput::GetPlayerInput( int index )
{
    return &mPlayerInputList[ index ];
}

int ShockBurnInput::GameInpInit( )
{
    // Get all inputs, which includes joysticks, buttons _and_ dip switches. Like, everything.
    
    // So the way it works is this:
    // All button inputs are individual structs.
    // The DIPS are organized by banks (DIPA, DIPB, DIPC)
    // and are flag BIT_DIPSWITCH.
    //
    // From the DIP banks, you then grab the individual dips from BurnDIPInfo()
    int i = 0;
    for( i = 0; i < MAX_BURN_INPUTS; i++ )
    {
        BurnInputInfo burnInputInfo;
        int result = BurnDrvGetInputInfo( &burnInputInfo, i );
        if ( result == 0 )
        {
            //PrintInputInfo( &burnInputInfo );
            
            mGameInputList[ i ].nType = burnInputInfo.nType;
            mGameInputList[ i ].pShortVal  = burnInputInfo.pShortVal;
            
            // dip bank?
            if( (burnInputInfo.nType & BIT_DIPSWITCH) == BIT_DIPSWITCH )
            {
                AssignInputWrapper( &mDipInputList[ mNumDipInputs ], &mGameInputList[ i ], &burnInputInfo );
                mNumDipInputs++;
            }
            else if( (burnInputInfo.nType & BIT_DIGITAL) == BIT_DIGITAL )
            {
                // is this a player input within player 1 - 9?
                if( tolower( burnInputInfo.szInfo[ 0 ] ) == 'p' 
                    && burnInputInfo.szInfo[ 1 ] >= '1' 
                    && burnInputInfo.szInfo[ 1 ] <= '9' )
                {
                    int playerIndex = atoi( &burnInputInfo.szInfo[ 1 ] ) - 1;                    
                    SetPlayerInput( playerIndex, &mGameInputList[ i ], &burnInputInfo );
                    mNumPlayers++;
                }
                else if ( !stricmp( burnInputInfo.szInfo, "diag" ) )
                {
                    AssignInputWrapper( &mDiagnosticInput, &mGameInputList[ i ], &burnInputInfo );
                }
                else if ( !stricmp( burnInputInfo.szInfo, "reset" ) )
                {
                    AssignInputWrapper( &mResetInput, &mGameInputList[ i ], &burnInputInfo );
                }
                else if ( !stricmp( burnInputInfo.szInfo, "service" ) )
                {
                    AssignInputWrapper( &mServiceInput, &mGameInputList[ i ], &burnInputInfo );
                }
            }
        }
        else
        {
            // this game has no more inputs.
            mNumTotalGameInputs = i;
            break;
        }
    }
}

void ShockBurnInput::SetPlayerInput( int playerIndex, GameInp *pGameInpInfo, BurnInputInfo *pBurnInputInfo )
{
    // jump past the "pN " part of the string so we know what type of input this is
    char *pTypePortion = pBurnInputInfo->szInfo + 3;
    
    //flushPrintf( "Setting Player Input for player: %d, type: %s\r\n", playerIndex, pTypePortion );
    
    // joystick input?
    if( !strncmp( "up", pTypePortion, sizeof( "up" ) ) )
    {
        //flushPrintf( "Player %d Joy Up Set\r\n", playerIndex );
        AssignInputWrapper( &mPlayerInputList[ playerIndex ].joyInput[ GameInp_Joy_Up ], 
                            pGameInpInfo, 
                            pBurnInputInfo );
    }
    else if ( !strncmp( "right", pTypePortion, sizeof( "right" ) ) )
    {
        //flushPrintf( "Player %d Joy Right Set\r\n", playerIndex );
        AssignInputWrapper( &mPlayerInputList[ playerIndex ].joyInput[ GameInp_Joy_Right ], 
                            pGameInpInfo, 
                            pBurnInputInfo );
    }
    else if ( !strncmp( "down", pTypePortion, sizeof( "down" ) ) )
    {
        //flushPrintf( "Player %d Joy Down Set\r\n", playerIndex );
        AssignInputWrapper( &mPlayerInputList[ playerIndex ].joyInput[ GameInp_Joy_Down ], 
                            pGameInpInfo, 
                            pBurnInputInfo );
    }
    else if ( !strncmp( "left", pTypePortion, sizeof( "left" ) ) )
    {
        //flushPrintf( "Player %d Joy Left Set\r\n", playerIndex );
        AssignInputWrapper( &mPlayerInputList[ playerIndex ].joyInput[ GameInp_Joy_Left ], 
                            pGameInpInfo, 
                            pBurnInputInfo );
    }
    // fire input?
    else if ( !strncmp( "fire ", pTypePortion, sizeof( "fire" ) ) )
    {
        //flushPrintf( "Begin fire index parse\r\n" );
        //flushPrintf( "Lookup fire index: %s\r\n", pTypePortion + 6 );
        
        // what fire index? We'll ignore any over GameInp_Fire_Count since thats all
        // the buttons we have
        int fireIndex = atoi( pTypePortion + 5 ) - 1;
        if( fireIndex < GameInp_Fire_Count )
        {
            AssignInputWrapper( &mPlayerInputList[ playerIndex ].fireInput[ fireIndex ], 
                                pGameInpInfo, 
                                pBurnInputInfo );
        }
        else
        {
            flushPrintf( "Warning: Ignoring fire input %d because we're out of room\r\n", fireIndex );
        }
    }
    // coin or start?
    else if ( !strncmp( "coin", pTypePortion, sizeof( "coin" ) ) )
    {
        //flushPrintf( "Player %d Coin\r\n", playerIndex );
        
        AssignInputWrapper( &mPlayerInputList[ playerIndex ].coinButton, 
                            pGameInpInfo,
                            pBurnInputInfo );
    }
    else if ( !strncmp( "start", pTypePortion, sizeof( "start" ) ) )
    {
        //flushPrintf( "Player %d Start\r\n", playerIndex );
        
        AssignInputWrapper( &mPlayerInputList[ playerIndex ].startButton, 
                            pGameInpInfo,
                            pBurnInputInfo );
    }
    else
    {
        flushPrintf( "Warning: Nothing found for input portion: %s\r\n", pTypePortion );
    }
}

void ShockBurnInput::AssignInputWrapper( GameInpWrapper *pInpWrapper, GameInp *pGameInpInfo, BurnInputInfo *pBurnInputInfo )
{    
    pInpWrapper->pGameInp = pGameInpInfo;
    
    strncpy( pInpWrapper->inputInfoName, 
                         pBurnInputInfo->szInfo, 
                         sizeof( pInpWrapper->inputInfoName ) );
                         
    strncpy( pInpWrapper->inputDisplayName, 
             pBurnInputInfo->szName, 
             sizeof( pInpWrapper->inputDisplayName ) );
}

void ShockBurnInput::InpDIPSWResetDIPs( )
{
    /* All told:
 * BurnDrvGetInputInfo
 * iterating this returns BurnInputInfo structs
 * with inputs and dip switch banks.
 * 
 * You copy the pointer pVal and the type int into your own 
 * master list of GameInp structs. (so its a long list with both game inputs and dip banks)
 * 
 * next you look thru all individual dips for 
 * one with a flagset to 0xF0 - this provides a memory offset
 * when trying to access an individual dip switch. Some games like
 * Knights dont have this offset, so its 0.
 * 
 * then you iterate the individual dip switches
 * and get its memory address (the individual bit(s) in the banks)
 * with this formula: SpecificDipInput = baseInputAddress + dipInfo.nInput + nDipOffset.
 * this is now a pointer to the individual dip within the dip bank. 
 * you then apply its default values.
 * */
    InpDIPSWGetOffset( );
    
	BurnDIPInfo bdi;

    int i = 0;
	while ( BurnDrvGetDIPInfo( &bdi, i ) == 0 )
	{
        // Is this dip in use?
		if (bdi.nFlags == 0xFF )
		{
            //PrintDIPInfo( &bdi );
            
            // jump our base struct pointer to this specific input.
            GameInp *pDipSwitch = mGameInputList + bdi.nInput + mDIPOffset;
            
            // TODO: Most emulators seem to simply _STORE_ the default value,
            // and not actually set it here. Revist.
            
            // apply its default value
            int tempVal = *pDipSwitch->pVal;
            *pDipSwitch->pVal = (tempVal & ~bdi.nMask) | (bdi.nSetting & bdi.nMask);
		}
        
		i++;
	}
}

void ShockBurnInput::InpDIPSWGetOffset( )
{
    // Checks to see if the game has an additional offset
    // that should be used when jumping to the memory address
    // for individual dips.
	BurnDIPInfo bdi;

	for(int i = 0; BurnDrvGetDIPInfo( &bdi, i ) == 0; i++)
	{
		if ( bdi.nFlags == 0xF0 )
		{   
			mDIPOffset = bdi.nInput;
			break;
		}
	}
}

void ShockBurnInput::PrintDIPInfo( BurnDIPInfo *pDipInfo )
{
    flushPrintf( "Burn DIP Info: \r\n"
                    "\tnInput: 0x%x\r\n"
                    "\tnFlags: 0x%x\r\n"
                    "\tnMask: 0x%x\r\n"
                    "\tnSetting: 0x%x\r\n"
                    "\tszText: %s\r\n",
                        pDipInfo->nInput,
                        pDipInfo->nFlags,
                        pDipInfo->nMask,
                        pDipInfo->nSetting,
                        pDipInfo->szText );
}

void ShockBurnInput::PrintInputInfo( BurnInputInfo *pInputInfo )
{
    flushPrintf( "Input: \r\n"
                    "\tName: %s\r\n"
                    "\tType: %d\r\n" //9 if it's BIT_DIPSWITCH
                    "\tVal:  0x%x, ShortVal: 0x%x\r\n"
                    "\tInfo: %s\r\n",
                        pInputInfo->szName,
                        pInputInfo->nType,
                        pInputInfo->pVal, pInputInfo->pShortVal,
                        pInputInfo->szInfo );
}
