
// See License.md for license

#include "../../includes.h"

void StateLoadError::Create( )
{
    UIBaseState::Create( );
    
    mRomLoadResult     = 0;
    mDisplayFatalError = 0;
    mExitEmulator      = 0;
    mExitUI            = 0;
}

void StateLoadError::Destroy( )
{
    UIBaseState::Destroy( );
}

void StateLoadError::EnterState( UIState oldState )
{
    UIBaseState::EnterState( oldState );
    
    // as we enter this state, lets figure out why we're being shown.
    // it was either a warning due to CRC/Len, or a fatal error we can't continue from.
    mRomLoadResult = ShockRomLoader::GetLoadResult();
    
    // if the success bit isn't set, this is a fatal error.
    if( (mRomLoadResult & LOAD_SUCCESS) == 0 )
    {
        mDisplayFatalError = 1;
    }
    else
    {
        mDisplayFatalError = 0;
    }
}

void StateLoadError::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
}

UIState StateLoadError::Update( )
{
    UIBaseState::Update( );
    
    if( mDisplayFatalError == 1 )
    {
        RenderFatalError( );
        
        // check for player acknowledgement
        if( ShockInput::GetInput( OptionsBack )->WasReleased( ) || 
            ShockInput::GetInput( P1_Red )->WasReleased() )
        {
            mExitEmulator = 1;
        }
    }
    else
    {
        RenderWarning( );
        
        // check for player acknowledgement
        if( ShockInput::GetInput( OptionsBack )->WasReleased( ) || 
            ShockInput::GetInput( P1_Red )->WasReleased() )
        {
            mExitUI = 1;
        }
    }
    
    // returning is different vs normal ui states, so don't use the base method
    return UIState_Count;
}

int StateLoadError::ShouldExitUI( )
{
    return mExitUI;
}

int StateLoadError::ShouldExitEmulator( )
{
    return mExitEmulator;
}

void StateLoadError::RenderFatalError( )
{
    int xPos = 0;
    int yPos = UI_Y_POS_MENU;
    
    char title[ MAX_PATH ] = { 0 };
    strncpy( title, "Load Error", sizeof( title ) );
    xPos = GetCenteredXPos( title );
    UIRenderer::DrawText( title, xPos, yPos, 0xFFFF );
        
    if( mRomLoadResult & LOAD_FATAL_GENERIC )
    {
        char message[ MAX_PATH ] = { 0 };
        snprintf( message, sizeof( message ), "%s could not be loaded", ShockRomLoader::GetRomsetName( ) );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
    }
    else if (mRomLoadResult & LOAD_FATAL_UNSUPPORTED )
    {
        char message[ MAX_PATH ] = { 0 };
        snprintf( message, sizeof( message ), "%s is not a supported romset", ShockRomLoader::GetRomsetName( ) );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
    }
    else if ( mRomLoadResult & LOAD_FATAL_MISSING_ROM )
    {
        char message[ MAX_PATH ] = { 0 };
        snprintf( message, sizeof( message ), "%s is missing roms:", ShockRomLoader::GetRomsetName( ) );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
        
        // this is a tough one; we need to look up all missing roms
        int numZipArchives = 0;
        RomsetZipArchive *pZips = ShockRomLoader::ErrorHandling_GetLoadedRomset_ZipArchives( &numZipArchives );
        
        int numRoms = 0;
        RomInfo *pRoms = ShockRomLoader::ErrorHandling_GetLoadedRoms( &numRoms );
        
        int i = 0;
        for( i = 0; i < numRoms; i++ )
        {
            if( ShockRomLoader::ErrorHandling_RomRequired( &pRoms[ i ] ) )
            {
                if( pRoms[ i ].romFileFound == 0 )
                {
                    char message[ MAX_PATH ] = { 0 };
                    snprintf( message, sizeof( message ), "%s", pRoms[ i ].burnRomInfo.szName );
                    xPos = GetCenteredXPos( message );
                    yPos += UI_ROW_HEIGHT / 2;
                    UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
                }
                
                // stop if we run out of screen space
                if ( yPos > PLATFORM_LCD_HEIGHT - (UI_ROW_HEIGHT * 2) )
                {
                    char message[ MAX_PATH ] = { 0 };
                    snprintf( message, sizeof( message ), "More..." );
                    xPos = GetCenteredXPos( message );
                    yPos += UI_ROW_HEIGHT / 2;
                    UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
                    break;
                }
            }
        }
    }
    
    UIBaseState::RenderBackOption( );
}

void StateLoadError::RenderWarning( )
{
    //TODO: Handle this. knightsu.zip is hacked so it'll work.
    UIBaseState::RenderBackOption( );
}
