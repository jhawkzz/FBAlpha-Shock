
// See License.md for license

#include "includes.h"

ShockState ShockMain::mState;
LoadResult ShockMain::mLoadResult;
char       ShockMain::mRomsetName[ MAX_PATH ];

OSTimer gGlobalTimer;

int ShockMain::Create( )
{
    mState      = ShockState_Loading;
    mLoadResult = LoadResult_None;
    
    // Setup Audio
    int result = ShockAudio::Create( );
    if( result == -1 )
    {
        flushPrintf( "ShockMain::CreateSystems() - Error, could not create Shock Audio!\r\n" );
        return -1;
    }
    
    // Setup Rendering
    result = ShockRenderer::Create( );
    if ( result == -1 )
    {
        flushPrintf( "ShockMain::CreateSystems() - Error, could not create Shock Renderer!\r\n" );
        return -1;
    }
   
    // Setup Input
    result = ShockInput::Create( );
    if( result == -1 )
    {
        flushPrintf( "ShockMain::CreateSystems() - Error, could not create Input! If Lubuntu, run with sudo\r\n" );
        return -1;
    }
    
#ifdef MVSX
    result = MVSXLed::Create( );
    if( result == -1 )
    {
        flushPrintf( "ShockMain::CreateSystems() - WARNING, could not create LED. Not fatal, will continue.\r\n" );
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
	BurnLibExit();
    
    //flushPrintf( "ShockUI::Destroy\r\n" );
    ShockUI::Destroy( );
    
#ifdef MVSX
    //flushPrintf( "MVSXLed::Destroy\r\n" );
    MVSXLed::Destroy( );
#endif
    
    //flushPrintf( "ShockRenderer::Destroy\r\n" );
    ShockRenderer::Destroy( );
        
    //flushPrintf( "ShockAudio::Destroy\r\n" );
    ShockAudio::Destroy( );
}

int ShockMain::BeginLoad( const char *pRomset )
{
    mState      = ShockState_Loading;
    mLoadResult = LoadResult_Pending;
    
    // show the UI in the load state
    ShockUI::SetState_Load( );
        
    strncpy( mRomsetName, pRomset, sizeof( mRomsetName ) );
    
    ShockThread t;
    int result = t.Create( ShockMain::LoadThread, mRomsetName );
    if( result != 0 )
    {
        flushPrintf( "ShockMain::BeginLoad() pthread_create failed with error: %d\r\n", result );
        return -1;
    }
    
    return 0;
}

int ShockMain::Run(const char* romSet)
{
    int result;
    
    gGlobalTimer.Reset();

    result = ShockMain::Create( );
    if ( result == -1 )
        return result;

    ShockMain::BeginLoad( romSet );
    
    do
    {
        ShockMain::Update( );
    }
    while (mState != ShockState_Quit);

    ShockMain::Destroy( );

    return 0;
}

void *ShockMain::LoadThread( void *pArg )
{
    LoadGameResult result = ShockGame::LoadGame( (char *)pArg );
    switch( result )
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
    
    switch( mLoadResult )
    {
        case LoadResult_Success:
        {
            // everything loaded, but were there warnings in the rom loader?
            if( (ShockRomLoader::GetLoadResult() & LOAD_WARNING_MASK) != 0
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

    if( result == 0 )
    {
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

    if( result == 0 )
    {
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
    if( ShockInput::GetInput( P1_Start )->GetState( ) == 1 &&
        ShockInput::GetInput( P1_Start )->GetTimeHeldMS( ) >= HOLD_TIME_FOR_FRONTEND_MILLI * MILLI_TO_MICROSECONDS )
    {
        mState = ShockState_FrontEnd;
        
        ShockUI::SetState_MainMenu( );
        ShockGame::Pause( 1 );
    }
}

void ShockMain::Update()
{
    int result = ShockMainCore::Update();

    if (result == -1)
    {
        mState = ShockState_Quit;
    }

    switch (mState)
    {
        case ShockState_Loading:
        {
            UpdateState_Loading( ); 
            break;
        }

        case ShockState_LoadError:
        {
            UpdateState_LoadError();
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
