// See License.md for license

#include "shock/core/ostimer.h"
#include "shock/input/shockinput.h"
#include "shock/shockprofiler.h"
#include "shock/ui/shockui.h"
#include "shock/ui/render/uirenderer.h"

UIState               ShockUI::mUIState;
StateMainMenu         ShockUI::mState_MainMenu;
StateLoadState        ShockUI::mState_LoadState;
StateSaveState        ShockUI::mState_SaveState;
StateButtonConfig     ShockUI::mState_ButtonConfig;
StateGameSettings     ShockUI::mState_GameSettings;
StateDisplaySettings  ShockUI::mState_DisplaySettings;
StateEmulatorSettings ShockUI::mState_EmulatorSettings;
StateCredits          ShockUI::mState_Credits;
StateLoading          ShockUI::mState_Loading;
StateLoadError        ShockUI::mState_LoadError;
UIBaseState          *ShockUI::mpStateList[ UIState_Count ];
UINT16                ShockUI::mBackgroundImage[ UI_WIDTH * UI_HEIGHT ];
UINT64                ShockUI::mFrameTimeMS;

void ShockUI::Create( )
{
    UIRenderer::Create( );
    
    // dynamically create our wallpaper
    UINT16 *pCurrBufferPos = mBackgroundImage;
    
    for( int y = 0; y < UI_HEIGHT; y++ )
    {
        for( int x = 0; x < UI_WIDTH; x++ )
        {
            // color the bg red
            pCurrBufferPos[ x ] = 0xB800;
        }
        
        pCurrBufferPos += UI_WIDTH;
    }
    
    // Render seperator line
    int xPos = 18;
    int yPos = UI_Y_POS_HEADER_SEPERATOR;
    int length = UI_WIDTH - xPos * 2;
    UINT16 *pShortBackBuffer = mBackgroundImage + (xPos + (yPos * UI_WIDTH));
        
    for( int x = 0; x < length; x++ )
    {
        pShortBackBuffer[ x ] = 0xFFFF;
    }
    
    mUIState = UIState_Count;
    
    memset( mpStateList, 0, sizeof( mpStateList ) );
    
    // setup our gamestate list
    mpStateList[ UIState_MainMenu ]         = &mState_MainMenu;
    mpStateList[ UIState_LoadState ]        = &mState_LoadState;
    mpStateList[ UIState_SaveState ]        = &mState_SaveState;
    mpStateList[ UIState_ButtonConfig ]     = &mState_ButtonConfig;
    mpStateList[ UIState_DisplaySettings ]  = &mState_DisplaySettings;
    mpStateList[ UIState_EmulatorSettings ] = &mState_EmulatorSettings;
    mpStateList[ UIState_GameSettings ]     = &mState_GameSettings;
    mpStateList[ UIState_Credits ]          = &mState_Credits;
    mpStateList[ UIState_Loading ]          = &mState_Loading;
    mpStateList[ UIState_LoadError ]        = &mState_LoadError;
    
    // initialize our gamestates
    for( int i = 0; i < UIState_Count; i++ )
    {
        if( mpStateList[ i ] != NULL )
        {
            mpStateList[ i ]->Create( );
        }
    }
    
    ChangeState( UIState_MainMenu );
}

void ShockUI::Destroy( )
{
    for( int i = 0; i < UIState_Count; i++ )
    {
        if( mpStateList[ i ] != NULL )
        {
            mpStateList[ i ]->Destroy( );
        }
    }
    
    UIRenderer::Destroy( );
}

void ShockUI::SetState_Load( )
{
    UIRenderer::Enable( );
    mFrameTimeMS = gGlobalTimer.GetElapsedTimeMicroseconds( );
    ChangeState( UIState_Loading );
}

void ShockUI::SetState_LoadError( )
{
    UIRenderer::Enable( );
    mFrameTimeMS = gGlobalTimer.GetElapsedTimeMicroseconds( );
    ChangeState( UIState_LoadError );
}

void ShockUI::SetState_MainMenu( )
{
    UIRenderer::Enable( );
    mFrameTimeMS = gGlobalTimer.GetElapsedTimeMicroseconds( );
    ChangeState( UIState_MainMenu );
}

int ShockUI::Update( )
{
    UINT64 totalTimeMS = gGlobalTimer.GetElapsedTimeMicroseconds();
    UINT64 tickRateMS  = SEC_TO_MICROSECONDS / 60;
    
    int uiActive = 1;
    
    UINT64 oldFrameTimeMS = mFrameTimeMS;

    while( mFrameTimeMS <= totalTimeMS )
    {
        // reset the renderlist each time we update
        // so we only render the final frame
        UIRenderer::ResetForFrame( );
        
        ShockInput::Update( );
        
        int stateResult = UpdateState();
        if ( stateResult < 1 )
        {
            uiActive = stateResult;
            break;
        }
        
        //AudioManager::Update( );
        
        mFrameTimeMS += tickRateMS;
    }

    if( oldFrameTimeMS != mFrameTimeMS )
    {
        UIRenderer::Render( );
        UIRenderer::Flip( );
    }
    
    return uiActive;
}

void ShockUI::ChangeState( UIState newState )
{
    // make sure we're IN a state (and not initially booting)
	if( mUIState != UIState_Count )
    {
        mpStateList[ mUIState ]->ExitState( newState );
    }
    
    UIState oldState = mUIState;
    mUIState = newState;
    
	mpStateList[ mUIState ]->EnterState( oldState );
}

int ShockUI::UpdateState( )
{
    // clear our back buffer to red
    UIRenderer::DrawBackground( mBackgroundImage );
    
    UIState newState = mpStateList[ mUIState ]->Update( );
    if( newState != UIState_Count )
    {
        if( mpStateList[ newState ] != NULL )
        {
            ChangeState( newState );
        }
    }    
    
    // special quit checks for the two states that can control
    // full app state
    if( mState_MainMenu.ShouldExitUI( )
     || mState_LoadError.ShouldExitUI( ) )
    {
        // put the state back to main menu for when we return
        ChangeState( UIState_MainMenu );
        return 0;
    }
    
    if( mState_MainMenu.ShouldExitEmulator( )
     || mState_LoadError.ShouldExitEmulator( ) )
    {
        // put the state back to main menu for when we return 
        // (tho it doesnt matter since we'll be exiting the app)
        ChangeState( UIState_MainMenu );
        return -1;
    }
    
    return 1;
}
