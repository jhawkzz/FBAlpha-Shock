
// See License.md for license

#ifndef SHOCKCONFIG_H_
#define SHOCKCONFIG_H_

#define MAX_GAMES   (9000)
#define CONFIG_FILE "fbaconfig.dat"

// we'll store settings, and MAX_GAMES worth of input configs (2 players per game)

struct SavedFireInput
{
    char romsetName[ MAX_PATH ];
    InputCodeToButtonMapping osInputToFireButtonLookup[ MAX_MVSX_PLAYERS ][ GameInp_Fire_Count ];
};

struct ConfigSettings
{
    int displayMode;
    int scanLines;
    int showFPS;
    int showLoadWarnings;
    int reserved[ 1020 ]; //for future use
    
    SavedFireInput savedConfigInputs[ MAX_GAMES ];
};

class ShockConfig
{
public:
    static void Create( );
    static void Destroy( );
    
    static void LoadConfigFile( );
    static int  SaveConfigFile( );
    
    static int  GetDisplayMode( );
    static void SetDisplayMode( int displayMode );
    
    static int  GetScanLinesEnabled( );
    static void SetScanLinesEnabled( int enabled );
    
    static int  GetShowFPS( );
    static void SetShowFPS( int enabled );
    
    static int  GetShowLoadWarnings( );
    static void SetShowLoadWarnings( int enabled );
    
    static SavedFireInput *LoadFireInputs( const char *pRomsetName );
    static void            SaveFireInputs( const char *pRomsetName, SavedFireInput *pFireInputs );
        
private:
    static void RestoreDefaults( );
    static int  GetConfigFilePath( char *pFilePath, int size );

private:
    static ConfigSettings mConfigSettings;
};

#endif