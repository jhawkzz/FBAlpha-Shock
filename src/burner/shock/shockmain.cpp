
// See License.md for license

#include "shock/core/core.h"
#include "shock/core/ostimer.h"
#include "shock/input/shockinput.h"
#include "shock/font/font.h"
#include "shock/shockprofiler.h"
#include "shock/shockaudio.h"
#include "shock/shockconfig.h"
#include "shock/shockgame.h"
#include "shock/shockmain.h"
#include "shock/shockromloader.h"
#include "shock/shockrenderer.h"
#include "shock/shocktimerdisplay.h"
#include "shock/ui/shockui.h"
#include "shock/util/util.h"

#ifdef MVSX_ASP
#include "shock/platform/core/mvsxled.h"
#endif

Thread      ShockMain::mLoadThread;
ShockState  ShockMain::mState;
LoadResult  ShockMain::mLoadResult;
char        ShockMain::mRomsetName[ MAX_PATH ];

OSTimer        gGlobalTimer;
ActivePlatform gActivePlatform;
char           gAssetPath[ MAX_PATH ];

int ShockMain::Create( )
{
    mState = ShockState_Loading;
    mLoadResult = LoadResult_None;

    // determine our platform
#ifdef WIN32
    gActivePlatform = ActivePlatform_Win32;
    getExeDirectory( gAssetPath, sizeof( gAssetPath ) );
#elif LUBUNTU
    gActivePlatform = ActivePlatform_Lubuntu;
    getExeDirectory( gAssetPath, sizeof( gAssetPath ) );
#elif MVSX_ASP
    // the MVSX has leds, so check for them
    if ( MVSXLed::DoesExist( ) )
    {
        gActivePlatform = ActivePlatform_MVSX;
    }
    else
    {
        gActivePlatform = ActivePlatform_ASP;
    }

    strncpy( gAssetPath, MVSX_ASP_ASSET_ROOT_PATH, sizeof( gAssetPath ) - 1 );

    // Create / ensure our assets root exists,
    struct stat st = { 0 };
    if ( stat( gAssetPath, &st ) == -1 )
    {
        int result = ShockCreateDir( gAssetPath );
        if ( result != -1 )
        {
            flushPrintf( "ShockMain() - Warning, could not create assets folder: %s\r\n", gAssetPath );
        }
    }
#endif

    // if somehow we don't know what platform we are, abort.
    if ( gActivePlatform == ActivePlatform_Count )
    {
        flushPrintf( "ShockMain::Create() - Error, could not determine active platform!\r\n" );
        return -1;
    }

    // Setup Audio
    int result = ShockAudio::Create( );
    if ( result == -1 )
    {
        flushPrintf( "ShockMain::Create() - Error, could not create Shock Audio!\r\n" );
        return -1;
    }

    // Setup Rendering
    result = ShockRenderer::Create( UI_WIDTH, UI_HEIGHT );
    if ( result == -1 )
    {
        flushPrintf( "ShockMain::Create() - Error, could not create Shock Renderer!\r\n" );
        return -1;
    }

    // Setup Input
    result = ShockInput::Create( );
    if ( result == -1 )
    {
        flushPrintf( "ShockMain::Create() - Error, could not create Input! If Lubuntu, run with sudo\r\n" );
        return -1;
    }

#ifdef MVSX_ASP
    if ( gActivePlatform == ActivePlatform_MVSX )
    {
        result = MVSXLed::Create( );
        if ( result == -1 )
        {
            flushPrintf( "ShockMain::Create() - WARNING, could not create LED. Not fatal, will continue.\r\n" );
        }
    }
#endif

    // Setup Font
    Font::Create( );

    // Setup UI
    ShockUI::Create( );

    return 0;
}

void ShockMain::Destroy( )
{
    ShockGame::UnloadGame( );

    //flushPrintf( "BurnLibExit\r\n" );
    BurnLibExit( );

    //flushPrintf( "ShockUI::Destroy\r\n" );
    ShockUI::Destroy( );

#ifdef MVSX_ASP
    //flushPrintf( "MVSXLed::Destroy\r\n" );
    if ( gActivePlatform == ActivePlatform_MVSX )
    {
        MVSXLed::Destroy( );
    }
#endif

    //flushPrintf( "ShockRenderer::Destroy\r\n" );
    ShockRenderer::Destroy( );

    //flushPrintf( "ShockAudio::Destroy\r\n" );
    ShockAudio::Destroy( );
}

int ShockMain::BeginLoad( const char *pRomset )
{
    mState = ShockState_Loading;
    mLoadResult = LoadResult_Pending;

    // show the UI in the load state
    ShockUI::SetState_Load( );

    strncpy( mRomsetName, pRomset, sizeof( mRomsetName ) );

    int result = mLoadThread.Create( ShockMain::LoadThread, mRomsetName );
    if ( result != 0 )
    {
        flushPrintf( "ShockMain::BeginLoad() pthread_create failed with error: %d\r\n", result );
        return -1;
    }

    return 0;
}

void ShockMain::Run( const char *pRomset )
{
    gGlobalTimer.Reset( );

    int result = ShockMain::Create( );
    if ( result == -1 )
    {
        flushPrintf( "ShockMain::Run() - ShockMain::Create() failed!\r\n" );
        return;
    }

    ShockMain::BeginLoad( pRomset );

    do
    {
        ShockTimerDisplay::Capture();
        TimerTree::Clear();

        ShockMain::Update( );

    } while ( mState != ShockState_Quit );

    ShockMain::Destroy( );
}

void *ShockMain::LoadThread( void *pArg )
{
    LoadGameResult result = ShockGame::LoadGame( (char *)pArg );
    switch ( result )
    {
    case LoadGameResult_Success:
    {
        mLoadResult = LoadResult_Success;
        break;
    }

    case LoadGameResult_Failed_Load:
    {
        mLoadResult = LoadResult_Failed_Load;
        break;
    }

    case LoadGameResult_Failed_Other:
    default:
    {
        mLoadResult = LoadResult_Failed_Other;

        flushPrintf( "ShockMain::LoadThread() Error, ShockGame::LoadGame for romset %s failed!"
            "(Not due to ShockRomLoader)\r\n",
            (char *)pArg );
        break;
    }
    }

    return NULL;
}

void ShockMain::UpdateState_Loading( )
{
    ShockUI::Update( );

    switch ( mLoadResult )
    {
    case LoadResult_Success:
    {
        // everything loaded, but were there warnings in the rom loader?
        if ( ( ShockRomLoader::GetLoadResult( ) & LOAD_WARNING_MASK ) != 0
            && ShockConfig::GetShowLoadWarnings( ) == 1 )
        {
            // goto the load error state, where they can continue on.
            ShockUI::SetState_LoadError( );

            mLoadResult = LoadResult_Count;
            mState = ShockState_LoadError;
        }
        // no issues whatsoever, launch the emulator
        else
        {
            ShockGame::Enable( );

            ShockGame::ResetFBATimer( );

            ShockRenderer::ClearBackBuffer( );

            mLoadResult = LoadResult_Count;
            mState = ShockState_Emulator;
        }
        break;
    }

    case LoadResult_Failed_Load:
    {
        ShockUI::SetState_LoadError( );

        mLoadResult = LoadResult_Count;
        mState = ShockState_LoadError;
        break;
    }

    case LoadResult_Failed_Other:
    {
        mState = ShockState_Quit;
        mLoadResult = LoadResult_Count;
        break;
    }
    }
}

void ShockMain::UpdateState_LoadError( )
{
    // a return of 1 means stay in the frontend load error screen
    // a return of 0 means continue to the emulator
    // a return of -1 means quit
    int result = ShockUI::Update( );

    if ( result == 0 )
    {
        ShockGame::Enable( );

        ShockGame::ResetFBATimer( );

        ShockRenderer::ClearBackBuffer( );

        mLoadResult = LoadResult_Count;
        mState = ShockState_Emulator;
    }
    else if ( result == -1 )
    {
        mState = ShockState_Quit;
    }
}

void ShockMain::UpdateState_FrontEnd( )
{
    // a return of 1 means stay in th frontend
    // a return of 0 means back to the emulator
    // a return of -1 means quit
    int result = ShockUI::Update( );

    if ( result == 0 )
    {
        ShockGame::Enable( );
        ShockRenderer::ClearBackBuffer( );
        ShockGame::Pause( 0 );

        mState = ShockState_Emulator;
    }
    else if ( result == -1 )
    {
        mState = ShockState_Quit;
    }
}

void ShockMain::UpdateState_Emulator( )
{
    ShockGame::Update( );

    // Check for switching to the frontend
    if ( ActivePlatform_MVSX == gActivePlatform )
    {
        // for mvsx, we need to make sure it was held, since there's no
        // standalone button for it
        if ( ShockInput::GetInput( OptionsMenu )->GetState( ) == 1 &&
            ShockInput::GetInput( OptionsMenu )->GetTimeHeldMS( ) >= HOLD_TIME_FOR_FRONTEND_MILLI * MILLI_TO_MICROSECONDS )
        {
            mState = ShockState_FrontEnd;

            ShockUI::SetState_MainMenu( );
            ShockGame::Pause( 1 );
        }
    }
    else
    {
        if ( ShockInput::GetInput( OptionsMenu )->WasReleased( ) )
        {
            mState = ShockState_FrontEnd;

            ShockUI::SetState_MainMenu( );
            ShockGame::Pause( 1 );
        }
    }
}

void ShockMain::Update( )
{
    SHOCK_PROFILE;

    int result = Core::Update();

    if ( result == -1 )
    {
        mState = ShockState_Quit;
    }

    switch ( mState )
    {
    case ShockState_Loading:
    {
        UpdateState_Loading( );
        break;
    }

    case ShockState_LoadError:
    {
        UpdateState_LoadError( );
        break;
    }

    case ShockState_FrontEnd:
    {
        UpdateState_FrontEnd( );
        break;
    }

    case ShockState_Emulator:
    {
        UpdateState_Emulator( );
        break;
    }
    }
}
