
// See License.md for license

#include "shock/shockconfig.h"
#include "shock/shockrenderer.h"
#include "shock/util/util.h"

SystemConfig ShockConfig::mSystemConfig;
char         ShockConfig::mGameAssetFolder[ MAX_PATH ];
GameConfig   ShockConfig::mGameConfig;

void ShockConfig::Create( )
{
    RestoreSystemDefaults( );
    RestoreGameDefaults( );
}

void ShockConfig::Destroy( )
{
}

int ShockConfig::GetConfigFilePath( char *pFilePath, int size )
{
    snprintf( pFilePath, size, "%s/%s", gAssetPath, CONFIG_FILE );
    return 0;
}

void ShockConfig::LoadSystemConfig( )
{
    char filePath[ MAX_PATH ] = { 0 };
    int result = GetConfigFilePath( filePath, MAX_PATH );
    if ( result == -1 )
    {
        flushPrintf( "ShockConfig::LoadSystemConfig() - Could not get Config File Path!\r\n" );
    }

    FILE *pFile = fopen( filePath, "rb" );
    if ( pFile != NULL )
    {
        int bytesRead = fread( &mSystemConfig, 1, sizeof( mSystemConfig ), pFile );
        if ( bytesRead != sizeof( mSystemConfig ) )
        {
            RestoreSystemDefaults( );

            flushPrintf( "ShockConfig::LoadSystemConfig() - Error, bytes read of %d did not match expected %d\r\n",
                bytesRead,
                sizeof( mSystemConfig ) );
        }

        fclose( pFile );
        pFile = NULL;
    }
    else
    {
        flushPrintf( "ShockConfig::LoadSystemConfig() - Warning, config file doesn't exist at: %s\r\n", filePath );
        flushPrintf( "This might just be the first time we've been run.\r\n", filePath );
    }
}

void ShockConfig::SaveSystemConfig( )
{
    char filePath[ MAX_PATH ] = { 0 };
    int result = GetConfigFilePath( filePath, MAX_PATH );
    if ( result == -1 )
    {
        flushPrintf( "ShockConfig::SaveSystemConfig() - Could not get Config File Path!\r\n" );
        return;
    }

    FILE *pFile = fopen( filePath, "wb" );
    if ( pFile != NULL )
    {
        int bytesWritten = fwrite( &mSystemConfig, 1, sizeof( mSystemConfig ), pFile );
        if ( bytesWritten != sizeof( mSystemConfig ) )
        {
            flushPrintf( "ShockConfig::SaveSystemConfig() - Error! Wrote %d bytes, but expected to write: %d\r\n",
                bytesWritten,
                sizeof( mSystemConfig ) );
        }

        fflush( pFile );
        fclose( pFile );
        pFile = NULL;
    }
    else
    {
        flushPrintf( "ShockConfig::SaveSystemConfig() - Error! Could not open config file at: %s\r\n", filePath );
    }
}

void ShockConfig::LoadGameConfig( const char *pRomset )
{
    char filePath[ MAX_PATH ] = { 0 };
    snprintf( filePath, sizeof( filePath ) - 1, "%s/%s.config", mGameAssetFolder, pRomset );

    FILE *pFile = fopen( filePath, "rb" );
    if ( pFile != NULL )
    {
        int bytesRead = fread( &mGameConfig, 1, sizeof( mGameConfig ), pFile );
        if ( bytesRead != sizeof( mGameConfig ) )
        {
            RestoreGameDefaults( );

            flushPrintf( "ShockConfig::LoadGameConfig() - Error, bytes read of %d did not match expected %d\r\n",
                bytesRead,
                sizeof( mGameConfig ) );
        }

        fclose( pFile );
        pFile = NULL;
    }
}

void ShockConfig::SaveGameConfig( const char *pRomset )
{
    char filePath[ MAX_PATH ] = { 0 };
    snprintf( filePath, sizeof( filePath ) - 1, "%s/%s.config", mGameAssetFolder, pRomset );

    FILE *pFile = fopen( filePath, "wb" );
    if ( pFile != NULL )
    {
        int bytesWritten = fwrite( &mGameConfig, 1, sizeof( mGameConfig ), pFile );
        if ( bytesWritten != sizeof( mGameConfig ) )
        {
            flushPrintf( "ShockConfig::SaveGameConfig() - Error! Wrote %d bytes, but expected to write: %d\r\n",
                bytesWritten,
                sizeof( mGameConfig ) );
        }

        fflush( pFile );
        fclose( pFile );
        pFile = NULL;
    }
    else
    {
        flushPrintf( "ShockConfig::SaveGameConfig() - Error! Could not open config file at: %s\r\n", filePath );
    }
}

void ShockConfig::RestoreSystemDefaults( )
{
    // set defaults - used on initial create and if the file fails to load
    // due to corruption
    memset( &mSystemConfig, 0, sizeof( mSystemConfig ) );
}

void ShockConfig::RestoreGameDefaults( )
{
    memset( &mGameConfig, 0, sizeof( mGameConfig ) );
}

int ShockConfig::GetDisplayMode( )
{
    return mGameConfig.displayMode;
}

void ShockConfig::SetDisplayMode( int displayMode )
{
    mGameConfig.displayMode = displayMode;
}

int ShockConfig::GetDisplayFilter( )
{
    return mGameConfig.displayFilter;
}

void ShockConfig::SetDisplayFilter( int displayFilter )
{
    mGameConfig.displayFilter = displayFilter;
}

int ShockConfig::GetShowFPS( )
{
    return mSystemConfig.showFPS;
}

void ShockConfig::SetShowFPS( int enabled )
{
    mSystemConfig.showFPS = enabled;
}

int ShockConfig::GetShowTimers( )
{
#ifdef SHOCK_PROFILERS
    return mConfigSettings.showTimers;
#else
    return 0;
#endif
}

void ShockConfig::SetShowTimers( int enabled )
{
    mSystemConfig.showTimers = enabled;
}

int ShockConfig::GetShowLoadWarnings( )
{
    return mSystemConfig.showLoadWarnings;
}

void ShockConfig::SetShowLoadWarnings( int enabled )
{
    mSystemConfig.showLoadWarnings = enabled;
}

SavedFireInput *ShockConfig::GetFireInputs( )
{
    //if this is the first time we've run this game, we dont have any settings to offer
    if ( mGameConfig.savedConfigInputs.isConfigured ) 
    {
        return &mGameConfig.savedConfigInputs;
    }

    return NULL;
}

void ShockConfig::SetFireInputs( SavedFireInput *pFireInputs )
{
    memcpy( mGameConfig.savedConfigInputs.fireButtonLookup,
        pFireInputs->fireButtonLookup,
        sizeof( mGameConfig.savedConfigInputs.fireButtonLookup ) );

    mGameConfig.savedConfigInputs.isConfigured = 1;
}

void ShockConfig::CreateGameAssetFolder( const char *pRomset )
{
    struct stat st = { 0 };

    // Game Folder
    snprintf( mGameAssetFolder, sizeof( mGameAssetFolder ), "%s/%s", gAssetPath, pRomset );
    if ( stat( mGameAssetFolder, &st ) == -1 )
    {
        int result = ShockCreateDir( mGameAssetFolder );
        if ( result == -1 )
        {
            flushPrintf( "ShockConfig::CreateGameAssetFolder() - WARNING, Unable to create Game Folder: %s\r\n", mGameAssetFolder );
        }
    }
}

const char *ShockConfig::GetGameAssetFolder( )
{
    return mGameAssetFolder;
}
