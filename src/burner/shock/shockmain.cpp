
// See License.md for license

#include "includes.h"

ShockState ShockMain::mState;
LoadResult ShockMain::mLoadResult;
char       ShockMain::mRomsetName[ MAX_PATH ];

int ShockMain::Create( )
{
    mState      = ShockState_Idle;
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
        flushPrintf( "ShockMain::CreateSystems() - Error, could not create Input! If x86, run with sudo\r\n" );
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

int ShockMain::BeginLoad( char *pRomset )
{
    mState      = ShockState_Loading;
    mLoadResult = LoadResult_Pending;
    
    // show the UI in the load state
    ShockUI::Activate( 1 );
        
    strncpy( mRomsetName, pRomset, sizeof( mRomsetName ) );
    
    pthread_t t;
    int result = pthread_create( &t, NULL, ShockMain::LoadThread, mRomsetName );
    if( result != 0 )
    {
        flushPrintf( "ShockMain::BeginLoad() pthread_create failed with error: %d\r\n", result );
        return -1;
    }
    
    pthread_detach( t );
    
    return 0;
}

int ShockMain::Update( )
{
    int result = 0;
    
    switch( mState )
    {
        case ShockState_Loading:
        {
            result = UpdateState_Loading( );        
            break;
        }
        
        case ShockState_Emulator:
        {
            result = UpdateState_Emulator( );
            break;
        }
        
        case ShockState_FrontEnd:
        {
            result = UpdateState_FrontEnd( );
            break;
        }
    }
    
    return result;
}

void *ShockMain::LoadThread( void *pArg )
{
    int result = ShockGame::LoadGame( (char *)pArg );
    if ( result == -1 )
    {
        flushPrintf( "ShockMain::LoadThread() Error, ShockGame::LoadGame for romset %s failed!\r\n", (char *)pArg );
        
        mLoadResult = LoadResult_Failed;
        return NULL;
    }
    
    mLoadResult = LoadResult_Success;

    return NULL;
}

int ShockMain::UpdateState_Loading( )
{
    int result = 0;
    
    ShockUI::Update( );
            
    if( mLoadResult == LoadResult_Success )
    {
        ShockGame::ResetFBATimer( );
    
        // print some game states cause all systems are GO!
        ShockGame::PrintGameInfo( );
        
        ShockRenderer::ClearBackBuffer( );
        
        mState = ShockState_Emulator;
    }
    else if ( mLoadResult == LoadResult_Failed )
    {
        result = -1;
    }
    
    return result;
}

int ShockMain::UpdateState_FrontEnd( )
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
        mState = ShockState_Idle;
    }
    
    return result;
}

int ShockMain::UpdateState_Emulator( )
{
    ShockGame::Update( );
            
    // Check for switching to the frontend
    if( ShockInput::GetInput( P1_Start )->GetState( ) == 1 &&
        ShockInput::GetInput( P1_Start )->GetTimeHeldMS( ) >= HOLD_TIME_FOR_FRONTEND_MILLI * MILLI_TO_MICROSECONDS )
    {
        mState = ShockState_FrontEnd;
        
        ShockUI::Activate( 0 );
        ShockGame::Pause( 1 );
    }
    
    return 0;
}
