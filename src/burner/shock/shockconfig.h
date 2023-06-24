
// See License.md for license

#ifndef SHOCKCONFIG_H_
#define SHOCKCONFIG_H_

#define CONFIG_FILE "fba.config"

#include "shock/input/shockplayerinput.h"

struct SavedFireInput
{
    int         isConfigured;
    ShockButton fireButtonLookup[ MAX_SHOCK_PLAYERS ][ GameInp_Fire_Count ];
};

struct SystemConfig
{
    int showFPS;
    int showLoadWarnings;
    int showTimers;
    int reserved[ 1024 ]; //for future use
};

struct GameConfig
{
    SavedFireInput savedConfigInputs;
    int displayMode;
    int displayFilter;
    int reserved[ 1024 ]; //for future use
};

class ShockConfig
{
public:
    static void Create( );
    static void Destroy( );

    static void LoadSystemConfig( );
    static void SaveSystemConfig( );

    static void CreateGameAssetFolder( const char *pRomset );
    static void LoadGameConfig( const char *pRomset );
    static void SaveGameConfig( const char *pRomset );

    static int  GetDisplayMode( );
    static void SetDisplayMode( int displayMode );

    static int  GetDisplayFilter( );
    static void SetDisplayFilter( int displayFilter );

    static int  GetShowFPS( );
    static void SetShowFPS( int enabled );

    static int  GetShowLoadWarnings( );
    static void SetShowLoadWarnings( int enabled );

    static int  GetShowTimers( );
    static void SetShowTimers( int enabled );

    static SavedFireInput *GetFireInputs( );
    static void            SetFireInputs( SavedFireInput *pFireInputs );

    static const char *GetGameAssetFolder( );

private:
    static void RestoreSystemDefaults( );
    static void RestoreGameDefaults( );
    static int  GetConfigFilePath( char *pFilePath, int size );

private:
    static char         mGameAssetFolder[ MAX_PATH ];
    static SystemConfig mSystemConfig;
    static GameConfig   mGameConfig;
};

#endif
