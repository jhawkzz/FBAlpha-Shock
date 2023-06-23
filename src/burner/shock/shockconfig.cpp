
// See License.md for license

#include "shock/shockconfig.h"
#include "shock/shockrenderer.h"
#include "shock/util/util.h"

ConfigSettings ShockConfig::mConfigSettings;

void ShockConfig::Create( )
{
    RestoreDefaults( );
}

void ShockConfig::Destroy( )
{
}

int ShockConfig::GetConfigFilePath( char *pFilePath, int size )
{
    snprintf( pFilePath, size, "%s/%s", gAssetPath, CONFIG_FILE );
    return 0;
}

void ShockConfig::LoadConfigFile( )
{
    char filePath[ MAX_PATH ] = { 0 };
    int result = GetConfigFilePath( filePath, MAX_PATH );
    if ( result == -1 )
    {
        flushPrintf( "ShockConfig::LoadConfigFile() - Could not get Config File Path!\r\n" );
    }

    FILE *pFile = fopen( filePath, "rb" );
    if ( pFile != NULL )
    {
        int bytesRead = fread( &mConfigSettings, 1, sizeof( mConfigSettings ), pFile );
        if ( bytesRead != sizeof( mConfigSettings ) )
        {
            RestoreDefaults( );

            flushPrintf( "ShockConfig::LoadConfigFile() - Error, bytes read of %d did not match expected %d\r\n",
                bytesRead,
                sizeof( mConfigSettings ) );
        }

        fclose( pFile );
        pFile = NULL;
    }
    else
    {
        flushPrintf( "ShockConfig::LoadConfigFile() - Warning, config file doesn't exist at: %s\r\n", filePath );
        flushPrintf( "This might just be the first time we've been run.\r\n", filePath );
    }
}

int ShockConfig::SaveConfigFile( )
{
    char filePath[ MAX_PATH ] = { 0 };
    int result = GetConfigFilePath( filePath, MAX_PATH );
    if ( result == -1 )
    {
        flushPrintf( "ShockConfig::SaveConfigFile() - Could not get Config File Path!\r\n" );
        return -1;
    }

    FILE *pFile = fopen( filePath, "wb" );
    if ( pFile != NULL )
    {
        int bytesWritten = fwrite( &mConfigSettings, 1, sizeof( mConfigSettings ), pFile );
        if ( bytesWritten != sizeof( mConfigSettings ) )
        {
            flushPrintf( "ShockConfig::SaveConfigFile() - Error! Wrote %d bytes, but expected to write: %d\r\n",
                bytesWritten,
                sizeof( mConfigSettings ) );
        }

        fflush( pFile );
        fclose( pFile );
        pFile = NULL;

        result = 0;
    }
    else
    {
        flushPrintf( "ShockConfig::SaveConfigFile() - Error! Could not open config file at: %s\r\n", filePath );
        result = -1;
    }

    return result;
}

void ShockConfig::RestoreDefaults( )
{
    // set defaults - used on initial create and if the file fails to load
    // due to corruption
    memset( &mConfigSettings, 0, sizeof( mConfigSettings ) );

    mConfigSettings.displayMode = ShockDisplayMode_FullScreen;
    mConfigSettings.displayFilter = ShockDisplayFilter_Pixel;
    mConfigSettings.showFPS = 0;
    mConfigSettings.showLoadWarnings = 0;
}

int ShockConfig::GetDisplayMode( )
{
    return mConfigSettings.displayMode;
}

void ShockConfig::SetDisplayMode( int displayMode )
{
    mConfigSettings.displayMode = displayMode;
}

int ShockConfig::GetDisplayFilter( )
{
    return mConfigSettings.displayFilter;
}

void ShockConfig::SetDisplayFilter( int displayFilter )
{
    mConfigSettings.displayFilter = displayFilter;
}

int ShockConfig::GetShowFPS( )
{
    return mConfigSettings.showFPS;
}

void ShockConfig::SetShowFPS( int enabled )
{
    mConfigSettings.showFPS = enabled;
}

int ShockConfig::GetShowTimers( )
{
#ifdef SHOCK_TIMERS
    return mConfigSettings.showTimers;
#else
    return 0;
#endif
}

void ShockConfig::SetShowTimers( int enabled )
{
    mConfigSettings.showTimers = enabled;
}

int ShockConfig::GetShowLoadWarnings( )
{
    return mConfigSettings.showLoadWarnings;
}

void ShockConfig::SetShowLoadWarnings( int enabled )
{
    mConfigSettings.showLoadWarnings = enabled;
}

SavedFireInput *ShockConfig::LoadFireInputs( const char *pRomsetName )
{
    SavedFireInput *pResult = NULL;

    // look for the game
    for ( int i = 0; i < MAX_GAMES; i++ )
    {
        if ( !strcmp( mConfigSettings.savedConfigInputs[ i ].romsetName, pRomsetName ) )
        {
            pResult = &mConfigSettings.savedConfigInputs[ i ];
            break;
        }
    }

    return pResult;
}

void ShockConfig::SaveFireInputs( const char *pRomsetName, SavedFireInput *pFireInputs )
{
    SavedFireInput *pInputForWrite = NULL;

    // look for the game or a blank entry we can use
    for ( int i = 0; i < MAX_GAMES; i++ )
    {
        if ( !strcmp( mConfigSettings.savedConfigInputs[ i ].romsetName, pRomsetName ) )
        {
            pInputForWrite = &mConfigSettings.savedConfigInputs[ i ];
            break;
        }
        // if we haven't yet found a blank entry, take the first one we find
        else if ( pInputForWrite == NULL && mConfigSettings.savedConfigInputs[ i ].romsetName[ 0 ] == 0 )
        {
            pInputForWrite = &mConfigSettings.savedConfigInputs[ i ];
        }
    }

    // it should never be null, but just in case
    if ( pInputForWrite != NULL )
    {
        strcpy( pInputForWrite->romsetName, pRomsetName );

        memcpy( pInputForWrite->fireButtonLookup,
            pFireInputs->fireButtonLookup,
            sizeof( pInputForWrite->fireButtonLookup ) );
    }
    else
    {
        flushPrintf( "ShockConfig::SaveFireInputs - Error, could not find slot to write for %s\r\n", pRomsetName );
    }
}
