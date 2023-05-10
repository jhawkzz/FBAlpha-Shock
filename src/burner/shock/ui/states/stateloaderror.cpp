
// See License.md for license

#include "../../includes.h"

void StateLoadError::Create( )
{
    UIBaseState::Create( );
    
    mRomLoadResult = 0;
    mExitEmulator  = 0;
    mExitUI        = 0;
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
}

void StateLoadError::ExitState( UIState newState )
{
    UIBaseState::ExitState( newState );
    
    mExitEmulator = 0;
    mExitUI       = 0;
}

UIState StateLoadError::Update( )
{
    UIBaseState::Update( );
    
    // Fatal error?
    if( (mRomLoadResult & LOAD_SUCCESS) == 0 )
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
    
    char message[ MAX_PATH ] = { 0 };
    strncpy( message, "Load Error", sizeof( message ) );
    xPos = GetCenteredXPos( message );
    UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
    
    snprintf( message, sizeof( message ), "Romset: %s", ShockRomLoader::GetRomsetName( ) );
    xPos = GetCenteredXPos( message );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
        
    if( mRomLoadResult & LOAD_FATAL_GENERIC )
    {
        snprintf( message, sizeof( message ), "Could not be load romset" );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
    }
    else if (mRomLoadResult & LOAD_FATAL_UNSUPPORTED )
    {
        snprintf( message, sizeof( message ), "Romset not supported" );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
    }
    else if ( mRomLoadResult & LOAD_FATAL_MISSING_ROM )
    {
        snprintf( message, sizeof( message ), "Roms Missing" );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
        
        snprintf( message, sizeof( message ), "Make sure the romset is" );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
        
        snprintf( message, sizeof( message ), "A parent, non-merged, or a clone with parent" );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
        
        snprintf( message, sizeof( message ), "Missing roms are:" );
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
                    snprintf( message, sizeof( message ), "%s", pRoms[ i ].burnRomInfo.szName );
                    xPos = GetCenteredXPos( message );
                    yPos += UI_ROW_HEIGHT / 2;
                    UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
                }
                
                // stop if we run out of screen space
                if ( yPos > PLATFORM_LCD_HEIGHT - (UI_ROW_HEIGHT * 2) )
                {
                    snprintf( message, sizeof( message ), "More..." );
                    xPos = GetCenteredXPos( message );
                    yPos += UI_ROW_HEIGHT / 2;
                    UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
                    break;
                }
            }
        }
    }
    
    UIBaseState::RenderBackOption( "Exit" );
}

void StateLoadError::RenderWarning( )
{
    int xPos = 0;
    int yPos = UI_Y_POS_MENU;
    
    char message[ MAX_PATH ] = { 0 };
    strncpy( message, "Load Warning", sizeof( message ) );
    xPos = GetCenteredXPos( message );
    UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
    
    snprintf( message, sizeof( message ), "Romset: %s", ShockRomLoader::GetRomsetName( ) );
    xPos = GetCenteredXPos( message );
    yPos += UI_ROW_HEIGHT / 2;
    UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
    
    if( mRomLoadResult & LOAD_WARNING_CRC )
    {
        snprintf( message, sizeof( message ), "CRC Validation" );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
        
        snprintf( message, sizeof( message ), "ROM CRC values did not match what was expected." );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
        
        snprintf( message, sizeof( message ), "This could simply mean this is a romhack." );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
    }
    
    if( mRomLoadResult & LOAD_WARNING_LEN )
    {
        snprintf( message, sizeof( message ), "Size Validation" );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
        
        snprintf( message, sizeof( message ), "Rom sizes did not match what was expected." );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
        
        snprintf( message, sizeof( message ), "This may not impact the ability to play" );
        xPos = GetCenteredXPos( message );
        yPos += UI_ROW_HEIGHT / 2;
        UIRenderer::DrawText( message, xPos, yPos, 0xFFFF );
    }
    
    UIBaseState::RenderBackOption( "Continue" );
}
