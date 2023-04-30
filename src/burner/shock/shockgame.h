
// See License.md for license

#ifndef SHOCKGAME_H_
#define SHOCKGAME_H_

#define GAME_BUFFER_WIDTH  (512)
#define GAME_BUFFER_HEIGHT (512)
#define GAME_BUFFER_BPP    (2)
#define MAX_SKIPPED_FRAMES (3) 
#define MAX_DRIFT_FRAMES   (15)
#define TICK_RESET_TIME    (60 * 60 * 20)

// used to track if we should flip a reset/diagnostic switch
enum GameInputSwitchState
{
    GameInputSwitchState_None       = -1,
    GameInputSwitchState_PendingOff =  0,
    GameInputSwitchState_PendingOn  =  1
};

class ShockGame
{
public:
    static int  LoadGame( const char *pRomset );
    static void UnloadGame( );
    
    static void ResetFBATimer( );
    static void Update( );
    static void Pause( int shouldPause );
    
    static int  GameHasDiagnosticMode( );
    static void SetDiagnosticMode( int shouldPress );
    static int  GetDiagnosticMode( );
    
    static int  GameHasReset( );
    static void SetReset( int shouldPress );
    static int  GetReset( );
    
    static void PrintGameInfo( );
    
private:
    static int  PrepareAudio( );
    static int  SetBurnSoundLen( int burnFPS );
    static int  PrepareRendering( );
    static void UpdateDiagnosticMode( );
    static void UpdateResetMode( );
    static void ResetFBATickTime( );
    static void ConfigurePaths( );

private:
    static int  mGameLoaded;
    static char mBurnAudioBuffer[ MAX_AUDIO_BUFFER_BYTES ];
    static char mGameBackBuffer[ GAME_BUFFER_WIDTH * GAME_BUFFER_HEIGHT * GAME_BUFFER_BPP ];
    static int  mGameDriverFlags;
    static int  mGameHardwareCode;
    static int  mGameWidth;
    static int  mGameHeight;
    static int  mPaused;
    
    static int  mFBA_FPS_Current;
    static int  mFBA_FPS_CurrTime;
    static int  mFBA_FPS_LastTime;
    static int  mFBA_FPS_FramesDrawn;
    
    // Timing - All timing is in Microseconds
    static int     mFBA_Timing_GameFrameTimeMS;  // the per-frame target tick time for the game
    static int     mFBA_Timing_StartingTickTime; // the timestamp when we began tracking the time (since last timer reset)
    static int     mFBA_Timing_CurrentTickTime;  // the timestamp for the current time
    static int     mFBA_Timing_Drift;
    static int     mFBA_Timing_MaxDrift;
    static int     mFBA_Timing_NumSkippedFrames;
    static int     mFBA_Timing_NumFramesTicked;
    static OSTimer mFBA_Timing_Timer;
    
    // Manage requests to enable disagnostic (settings) menu / reset the game
    static GameInputSwitchState mDiagnosticMode;
    static GameInputSwitchState mReset;
};

#endif
