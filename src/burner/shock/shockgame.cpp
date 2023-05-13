
// See License.md for license

#include "includes.h"

int  ShockGame::mGameLoaded;
char ShockGame::mBurnAudioBuffer[ MAX_AUDIO_BUFFER_BYTES ];
char ShockGame::mGameBackBuffer[ GAME_BUFFER_WIDTH * GAME_BUFFER_HEIGHT * GAME_BUFFER_BPP ];
int  ShockGame::mGameDriverFlags;
int  ShockGame::mGameHardwareCode;
int  ShockGame::mGameWidth;
int  ShockGame::mGameHeight;
int  ShockGame::mPaused;
int  ShockGame::mFBA_FPS_Current;
int  ShockGame::mFBA_FPS_CurrTime;
int  ShockGame::mFBA_FPS_LastTime;
int  ShockGame::mFBA_FPS_FramesDrawn;
int  ShockGame::mFBA_Timing_GameFrameTimeMS;
int  ShockGame::mFBA_Timing_CurrentTickTime;
int  ShockGame::mFBA_Timing_StartingTickTime;
int  ShockGame::mFBA_Timing_Drift;
int  ShockGame::mFBA_Timing_MaxDrift;
int  ShockGame::mFBA_Timing_NumSkippedFrames;
int  ShockGame::mFBA_Timing_NumFramesTicked;
OSTimer              ShockGame::mFBA_Timing_Timer;
GameInputSwitchState ShockGame::mDiagnosticMode;
GameInputSwitchState ShockGame::mReset;

//***Start Burn required implementations
TCHAR szAppEEPROMPath[MAX_PATH];
TCHAR szAppHiscorePath[MAX_PATH];
TCHAR szAppBlendPath[MAX_PATH];
TCHAR szAppSamplesPath[MAX_PATH]; // Not used
int   kNetGame = 0;               // Required by neo_upd4990a.cpp

void Reinitialise( void ) 
{ 
}

UINT32 __cdecl HighCol16(INT32 r, INT32 g, INT32 b, INT32 /* i */);

extern UINT8 *pBurnDraw;
//***End Burn required implementations

LoadGameResult ShockGame::LoadGame( const char *pRomset )
{
    // Grab the config file (we need it early, it controls CRC enforcement)
    ShockConfig::Create( );
    ShockConfig::LoadConfigFile( );
    
    // Provide FBA with the paths for file i/o
    ConfigurePaths( );
    
    InitHiscoreSupport( );
    
    // initialize FBA's core lib (figures out what games are available)
    BurnLibInit( );
    
    // load the romset
    int result = ShockRomLoader::LoadRomset( pRomset );
    if ( result == -1 )
    {
        flushPrintf( "ShockGame::LoadGame() - Romset %s not valid. Cannot continue.\r\n", ShockRomLoader::GetRomsetName( ) );
        return LoadGameResult_Failed_Load;
    }
    
    // FBA needs  a few things set PRIOR to initializing the driver,
    // and then some of them set AGAIN after initialization.
    nBurnBpp = GAME_BUFFER_BPP;
    
    nBurnSoundRate   = SAMPLE_RATE;
    pBurnSoundOut    = (int16_t *)mBurnAudioBuffer;
    nFMInterpolation = 2;
	nInterpolation   = 2;
    SetBurnSoundLen( nBurnFPS ); //This will get set _AGAIN_ post init
    
    // set FBA's pallette recalc callback
    BurnHighCol = HighCol16;
    
    ShockBurnInput::Create( );
    
    // game is loaded, now have FBA prepare everything for the game
    BurnDrvInit( );
    
    // Burn moves the games FPS decimal place to the right two places
    // so we can do fixed point math. (no floating point)
    //
    // So we will do the same for our microseconds, then calculate the
    // target frame time
    // Example Timing: Microsec*100 / (5962 - 10) for xmvsf = 16801
    // Example Timing: Microsec*100 / (6000 - 30) for sf3 = 16863
    // why are they subtracting either 30 or 10 from the FPS before dividing?
    mFBA_Timing_GameFrameTimeMS = (1000000*100) / (nBurnFPS - (nBurnFPS >= 6000 ? 30 : 10));
    mFBA_Timing_MaxDrift        = MAX_DRIFT_FRAMES * mFBA_Timing_GameFrameTimeMS;
    
    result = PrepareRendering( );
    if( result == -1 )
    {
        flushPrintf( "ShockGame::LoadGame() - Failed to prepare rendering!\r\n" );
        return LoadGameResult_Failed_Other;
    }
    
    result = PrepareAudio( );
    if( result == -1 )
    {
        flushPrintf( "ShockGame::LoadGame() - Failed to prepare audio!\r\n" );
        return LoadGameResult_Failed_Other;
    }
    
    // everything is now setup - perform wrap up tasks now that we know what game we're playing
    mGameHardwareCode = BurnDrvGetHardwareCode();
    
    ShockPlayerInput::Create( mGameHardwareCode );
    
    ShockPlayerInput::LoadFireInputs( ShockRomLoader::GetRomsetName( ) );
    
#ifdef MVSX
    MVSXLed::SetLED( 0, 0 );
    MVSXLed::SetLED( 1, 0 );
#endif

    mDiagnosticMode = GameInputSwitchState_None;
    mReset          = GameInputSwitchState_None;
    
    mGameLoaded = 1;
   
    return LoadGameResult_Success;
}

void ShockGame::UnloadGame( )
{
    if( mGameLoaded )
    {
        //flushPrintf( "BurnDrvExit\r\n" );
        BurnDrvExit();

        ShockPlayerInput::SaveFireInputs( ShockRomLoader::GetRomsetName( ) );
        
        //flushPrintf( "SaveConfigFile::Destroy\r\n" );
        ShockConfig::SaveConfigFile( );
    }
}

void ShockGame::ResetFBATimer( )
{
    //flushPrintf("Reset FBA Timer\r\n");
    
    // reset the actual timer (prevent flip due to microseconds being such high precision)
    mFBA_Timing_Timer.Reset();
    
    // reset our tick timing values
    ResetFBATickTime( );
    
    // reset our drift time
    mFBA_Timing_Drift = 0;
    
    // and our FPS
    mFBA_FPS_LastTime    = mFBA_Timing_CurrentTickTime;
    mFBA_FPS_CurrTime    = mFBA_Timing_CurrentTickTime;
    mFBA_FPS_Current     = 0;
    mFBA_FPS_FramesDrawn = 0;
}

void ShockGame::Update( )
{
    //todo - this isnt great, obviously, but works well enough
    if( mPaused == 1 )
    {
        ShockPlayerInput::Update( );
        return;
    }
    
    // Monitor FPS - Update once every second, or when our timer flips
    mFBA_FPS_CurrTime = mFBA_Timing_Timer.GetElapsedTimeMicroseconds( );
    if( mFBA_FPS_CurrTime - mFBA_FPS_LastTime > SEC_TO_MICROSECONDS || mFBA_FPS_CurrTime < mFBA_FPS_LastTime)
    {
        mFBA_FPS_Current     = mFBA_FPS_FramesDrawn;
        mFBA_FPS_LastTime    = mFBA_FPS_CurrTime;
        mFBA_FPS_FramesDrawn = 0;
    }
    
    // get elapsed time since the last time ticking was reset,
    // which is at: start of game, when max skipped frames is hit, or when Reset Ticks is hit.
    mFBA_Timing_CurrentTickTime = mFBA_Timing_Timer.GetElapsedTimeMicroseconds( );

    // This is our drift. Are we running ahead of the game's frame time, or have we fallen behind?
    // The drift is relative to the last time ticking was fully reset via ResetFBATickTime();
    // which is at start of game, when max skipped frames is hit, or the check below.
    //
    // mFBA_Timing_Drift is actual time we've been ticking minus time the game has been running.
    // In a perfect world this would be 0.
    mFBA_Timing_Drift = (mFBA_Timing_CurrentTickTime - mFBA_Timing_StartingTickTime)
                        - (mFBA_Timing_NumFramesTicked * mFBA_Timing_GameFrameTimeMS);
    
    // Should we reset everything?
    //1: If our timer has flipped
    //2: About once ever 20 minutes
    //3: If the drift goes ahead / falls behind 15 frames of game time
    if( abs( mFBA_Timing_Drift ) > mFBA_Timing_MaxDrift 
        || mFBA_Timing_NumFramesTicked > TICK_RESET_TIME
        || mFBA_Timing_CurrentTickTime < mFBA_Timing_StartingTickTime )
    {
        ResetFBATimer( );
    }
    
    mFBA_Timing_NumFramesTicked++;
    
    // This will always be true on our very first tick
    if( mFBA_Timing_Drift > 0 && mFBA_Timing_NumSkippedFrames < MAX_SKIPPED_FRAMES )
    {
        pBurnDraw = NULL;
        
        BurnDrvFrame( );
        
        if( pBurnSoundOut )
        {
            ShockAudio::PlayBuffer( (char*)pBurnSoundOut, nBurnSoundLen * SAMPLE_BLOCK_ALIGN );
        }
        
        mFBA_Timing_NumSkippedFrames++;
    }
    else
    {
        // get the exact real time in microseconds before we tick.
        // This will capture the microseconds from now til we're done with the game tick.
        UINT32 frameStartTime = mFBA_Timing_Timer.GetElapsedTimeMicroseconds( );
                
        pBurnDraw = (uint8_t*)ShockGame::mGameBackBuffer;
    
        BurnDrvFrame();
        
        if( pBurnSoundOut )
        {
            ShockAudio::PlayBuffer( (char*)pBurnSoundOut, nBurnSoundLen * SAMPLE_BLOCK_ALIGN );
        }
        
        ShockRenderer::RenderFBA( (UINT16 *)mGameBackBuffer, 
                                  mGameWidth, 
                                  mGameHeight, 
                                  mGameDriverFlags,
                                  mFBA_FPS_Current );
        ShockRenderer::Flip( );
        
        mFBA_FPS_FramesDrawn++;
        
        if( mFBA_Timing_NumSkippedFrames < MAX_SKIPPED_FRAMES )
        {
            // get exactly how much time it took to tick the frame
            UINT32 frameEndTime = mFBA_Timing_Timer.GetElapsedTimeMicroseconds( );
                        
            // watch for timer flip
            if(frameEndTime < frameStartTime) 
            {
                frameStartTime = frameEndTime;
            }
            
            // Sleep if we are AHEAD of game time.
            // We take frameTime (xmvsf: 16801) - deltaTime - drift - (HALF Frame Time) [This guarantees we never sleep for 
            // more than HALF a game frame]
            int frameDeltaTime = frameEndTime - frameStartTime;
            int halfGameTime   = mFBA_Timing_GameFrameTimeMS / 2;
            int sleepTime      = mFBA_Timing_GameFrameTimeMS - frameDeltaTime - mFBA_Timing_Drift - halfGameTime;
            
            // only sleep if we should for more than 1 millisecond.
            // otherwise the overhead of usleep() would take more time.
            if( sleepTime > MILLI_TO_MICROSECONDS )
            {
               ShockSleep( sleepTime );
            }
        }
        else
        {
            // Max Frames have been skipped. Reset tick timing.
            ResetFBATickTime( );
        }
        
        mFBA_Timing_NumSkippedFrames = 0;
    }
    
    ShockPlayerInput::Update( );
    
    UpdateDiagnosticMode( );
    UpdateResetMode( );
}

void ShockGame::Pause( int shouldPause )
{
    // if we're paused and will be unpausing, 
    // reset ticks
    if( shouldPause != mPaused )
    {
        // if we're paused and should unpause, reset our timer
        if( shouldPause == 0 && mPaused == 1 )
        {
            ResetFBATimer( );
        }
        
        mPaused = shouldPause;
    }
}

int ShockGame::GameHasDiagnosticMode( )
{
    return ShockBurnInput::GameHasDiagnosticMode( );
}

void ShockGame::SetDiagnosticMode( int shouldPress )
{
    // this should be called whle the game is not ticking,
    // and when it does tick, it will be processed
    mDiagnosticMode = shouldPress == 1 ? GameInputSwitchState_PendingOn : GameInputSwitchState_PendingOff;
}

int ShockGame::GetDiagnosticMode( )
{
    return mDiagnosticMode == GameInputSwitchState_PendingOn;
}

int ShockGame::GameHasReset( )
{
    return ShockBurnInput::GameHasReset( );
}

void ShockGame::SetReset( int shouldPress )
{
    mReset = shouldPress == 1 ? GameInputSwitchState_PendingOn : GameInputSwitchState_PendingOff;
}

int ShockGame::GetReset( )
{
    return mReset == GameInputSwitchState_PendingOn;
}

void ShockGame::PrintGameInfo( )
{
    flushPrintf( "-=-=-=-=Game Info=-=-=-=-\r\n" );
    flushPrintf( "Game Romset: %s\r\n"
                 "Game Flags: 0x%x\r\n"
                 "Game Resolution: %d x %d\r\n"
                 "Game FPS: %d\r\n" 
                 "Game Audio Samples Per Frame: %d\r\n",
                    ShockRomLoader::GetRomsetName( ),
                    mGameDriverFlags,
                    mGameWidth, mGameHeight,
                    nBurnFPS,
                    nBurnSoundLen );
}

int ShockGame::PrepareAudio( )
{
    int result = SetBurnSoundLen( nBurnFPS );
    if( result < 0 )
    {
        flushPrintf( "ShockGame::PrepareAudio() - Failed to caculate burn sound length.\r\n" );
        return -1;
    }

    return 0;   
}

int ShockGame::SetBurnSoundLen( int burnFPS )
{
    // The frame samples per tick needs to be based off
    // the game's FPS, and most implementations use this formula:
    nBurnSoundLen = ( nBurnSoundRate * 100 + (burnFPS >> 1) ) / burnFPS;
    
    // sanity check
    int bufferLength = nBurnSoundLen * SAMPLE_BLOCK_ALIGN;
    if( bufferLength > MAX_AUDIO_BUFFER_BYTES )
    {
        flushPrintf( "ShockAudio::CalcBurnSoundLen() - Needed buffer length more than supported. Need: %d Have: %d\r\n", 
                     bufferLength, 
                     MAX_AUDIO_BUFFER_BYTES );
        return -1;
    }
    
    return 0;
}

int ShockGame::PrepareRendering( )
{
    BurnDrvGetVisibleSize( &mGameWidth, &mGameHeight );
    
	// figure out orientation
    mGameDriverFlags = BurnDrvGetFlags( );
	if ( mGameDriverFlags & BDF_ORIENTATION_VERTICAL )
    {
		int temp    = mGameWidth;
		mGameWidth  = mGameHeight;
		mGameHeight = temp;
	}
    
    // set FBA's pitch
	nBurnPitch = mGameWidth * nBurnBpp;
    
    // have FBA update the palette so we can run in 16 bit color
	BurnRecalcPal( );
    
    // prep our back buffer
    if( mGameWidth > GAME_BUFFER_WIDTH || mGameHeight > GAME_BUFFER_HEIGHT )
    {
        flushPrintf( "ShockGame::Create() Error - Game size too large for buffer." 
                     "Max Width: %d, Max Height: %d, Game Width: %d, Game Height: %d\r\n", 
                     GAME_BUFFER_WIDTH, 
                     GAME_BUFFER_HEIGHT, 
                     mGameWidth, 
                     mGameHeight );
            
        return -1;
    }
    
    return 0;
}

void ShockGame::UpdateDiagnosticMode( )
{
    if( mDiagnosticMode == GameInputSwitchState_PendingOn )
    {
        //*JHM NOTE: This FBA's Neo Geo driver not support Neo Geo Unibios > 3.0 (so no 3.3, 4.0)
        // However, 4.0 is widely used with this emulator. There's a memory overwrite due
        // to this, and one way it shows up is when p1/p2 coin input is written to.
        // 
        // If its written to and then the test menu is accessed, some games crash. (KOF 94)
        // The right fix is to upgrade the driver to support Unibios 4.0.
        // Until then, here's an ugly hack.
        //
        // Hit the reset game flag, which clears out its memory (bye bye corrupt memory)
        // advance the emulator one frame to process the reset
        // fast forward to skip past the unibios boot up
        //
        // NOW we can turn on the diagnostic mode
        /*if( (mGameHardwareCode & HARDWARE_PUBLIC_MASK) == HARDWARE_SNK_NEOGEO
        ||  (mGameHardwareCode & HARDWARE_PUBLIC_MASK) == (HARDWARE_PREFIX_CARTRIDGE | HARDWARE_SNK_NEOGEO) )
        {
            pBurnDraw = NULL;
        
            ShockBurnInput::ToggleReset( 1 );
            BurnDrvFrame( );
            ShockBurnInput::ToggleReset( 0 );
            for( int i = 0; i < 600; i++ )
            {
                BurnDrvFrame();
            }
            
            pBurnDraw = (uint8_t*)ShockGame::mGameBackBuffer;
        }*/
        
        ShockBurnInput::ToggleDiagnosticMode( 0, 1 );
        mDiagnosticMode = GameInputSwitchState_PendingOff;
    }
    else if ( mDiagnosticMode == GameInputSwitchState_PendingOff )
    {
        ShockBurnInput::ToggleDiagnosticMode( 0, 0 );
        mDiagnosticMode = GameInputSwitchState_None;
    }
}

void ShockGame::UpdateResetMode( )
{
    if ( mReset == GameInputSwitchState_PendingOn )
    {
        ShockBurnInput::ToggleReset( 1 );
        mReset = GameInputSwitchState_PendingOff;
    }
    else if ( mReset == GameInputSwitchState_PendingOff )
    {
        ShockBurnInput::ToggleReset( 0 );
        mReset = GameInputSwitchState_None;
    }
}

void ShockGame::ResetFBATickTime( )
{
    // reset our game tick timing. These are used to monitor drift
    mFBA_Timing_StartingTickTime = mFBA_Timing_Timer.GetElapsedTimeMicroseconds( );
    mFBA_Timing_CurrentTickTime  = mFBA_Timing_StartingTickTime;
    mFBA_Timing_NumFramesTicked  = 0;
    mFBA_Timing_NumSkippedFrames = 0;
}

void ShockGame::ConfigurePaths( )
{
    struct stat st = { 0 };
    
    char path[ MAX_PATH ] = { 0 };
    int result = getExeDirectory( path, MAX_PATH );
    if( result == -1 )
    {
        flushPrintf( "ShockGame::ConfigurePaths() - ERROR, Unable to get exe path\r\n" );
        return;
    }
    
    // EEPROM
    snprintf( szAppEEPROMPath, sizeof( szAppEEPROMPath ), "%s/" EEPROM_PATH, path );
    if ( stat( szAppEEPROMPath, &st ) == -1 ) 
    {
        int result = ShockCreateDir( szAppEEPROMPath );
        if( result == -1 )
        {
            flushPrintf( "ShockGame::ConfigurePaths() - WARNING, Unable to create EEPROM_PATH: %s\r\n", szAppEEPROMPath );   
        }
    }
    
    // Hiscore
    snprintf( szAppHiscorePath, sizeof( szAppHiscorePath ), "%s/" HISCORE_PATH, path );
    if ( stat( szAppHiscorePath, &st ) == -1 ) 
    {
        int result = ShockCreateDir( szAppHiscorePath );
        if( result == -1 )
        {
            flushPrintf( "ShockGame::ConfigurePaths() - WARNING, Unable to create HISCORE_PATH: %s\r\n", szAppHiscorePath );   
        }
    }
    
    // JHM: TODO - Add these tables
    // Blend
    snprintf( szAppBlendPath, sizeof( szAppBlendPath ), "%s/" BLEND_PATH, path );
    if ( stat( szAppBlendPath, &st ) == -1 ) 
    {
        int result = ShockCreateDir( szAppBlendPath );
        if( result == -1 )
        {
            flushPrintf( "ShockGame::ConfigurePaths() - WARNING, Unable to create BLEND_PATH: %s\r\n", szAppBlendPath );   
        }
    }
}

void ShockGame::InitHiscoreSupport( )
{
    // FBA depends on a file called "hiscore.dat", a text file that
    // contains per-game memory mappings for hiscores. For portability,
    // we bundle it in hiscoredat.h and write it out if it doesn't exist on the
    // system.
    
    char path[ MAX_PATH ] = { 0 };
    int result = getExeDirectory( path, MAX_PATH );
    if( result == -1 )
    {
        flushPrintf( "ShockGame::InitHiscoreSupport() - ERROR, Unable to get exe path\r\n" );
        return;
    }
        
    // does the hiscore.dat file exist?
    char hiscoreDatFilePath[ MAX_PATH ] = { 0 };
    snprintf( hiscoreDatFilePath, sizeof( hiscoreDatFilePath ), "%s" HISCORE_DAT_FILENAME, szAppHiscorePath );
    
    FILE *pFile = fopen( hiscoreDatFilePath, "r" );
    if( pFile == NULL )
    {
        // write ours out
        pFile = fopen( hiscoreDatFilePath, "w" );
        if( pFile != NULL )
        {
            fprintf( pFile, "%s", hiscoreDat );
            fclose( pFile );
        }
        else
        {
            flushPrintf( "ShockGame::InitHiscoreSupport() - WARNING, Unable to create %s\r\n", hiscoreDatFilePath );
            return;
        }
    }
    
    // made it to the end; enable hiscore support
    EnableHiscores = 1;
}
