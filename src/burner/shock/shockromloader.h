
// See License.md for license

#ifndef SHOCKROMLOADER_H_
#define SHOCKROMLOADER_H_

#define MAX_ROMS        (256)
#define MAX_ROMSET_ZIPS (64) //Struggling to imagine a game split into more zips than this.

#define ROMCHECK_BAD_CRC       (0x1)
#define ROMCHECK_TOO_LARGE     (0x1 << 1)
#define ROMCHECK_TOO_SMALL     (0x1 << 2)
#define ROMCHECK_NAME_MISMATCH (0x1 << 3)

#define LOAD_SUCCESS           (0x1)      //if this is set, we can move forward and the fatal bits will *NOT* be set.
#define LOAD_FATAL_GENERIC     (0x1 << 1) //if this is set, its an error we cant recover from and the SUCCESS bit will *NOT* be set.
#define LOAD_FATAL_UNSUPPORTED (0x1 << 2) //if this is set, its an error we cant recover from and the SUCCESS bit will *NOT* be set.
#define LOAD_FATAL_MISSING_ROM (0x1 << 3) 
#define LOAD_WARNING_CRC       (0x1 << 4) //if this is set, there were crc issues (but they can be ignored)
#define LOAD_WARNING_LEN       (0x1 << 5) //if this is set, there were size issues (but they can be ignored)
#define LOAD_WARNING_MASK      (LOAD_WARNING_CRC | LOAD_WARNING_LEN)

// Represents an individual rom within a Romset.
// Really worth reading this: https://docs.mamedev.org/usingmame/aboutromsets.html
// A romset can be structured as follows:
// Nonmerged - a fully self contained game. All roms in one zip file.
// Parent - A fully self contained game. All roms in one zip file.
// Clone - A subset of just the roms that, when combined with the parent, create a specific version of the game.
//
// Example: X-Men Children of the Atom
// xmcota.zip is the parent. This is the European release and is self contained.
// xmcotaj.zip is a clone. This is the Japanese release, but only contains the roms
// that are changed. When these are combined with xmcota.zip 
// (meaning when a rom exists in both, we use the xmcotaJ version, and we also add in roms that are only in xmcotaJ)
// we end up with the Japanese version.
//
// A nonmerged set would be the same as the above, but all the roms are in one zip.
// So, for xmcotaj.zip, that's all you need, it has everything. FBA will still tell you
// that it has a dependency on xmcota.zip, but everything in xmcotaj.zip will override.
struct RomInfo
{
    // if this is 0, we literally could not find the rom in any zip archive.
    int romFileFound;
    
    // 0 means no problem, bitmasks above indicate issues.
    int romCheck; 
    
    // this is the index to the zip archive with this rom.
    int zipArchiveIndex;
    
    // this is the index of the rom within the given zip.
    int indexInZipArchive;
    
    int foundCrc;
    
    int foundLen;
    
    char foundName[ MAX_PATH ];
    
    // given to us by FBA, this contains the source of truth for the rom.
    BurnRomInfo burnRomInfo;
};

struct RomsetZipArchive
{
    char zipArchiveName[ MAX_PATH ];
    int  zipExists;
};

struct ValidationResult
{
    int crcError;
    int lenError;
    int missingRomError;
};

class ShockRomLoader
{
public:
    static int         LoadRomset( const char *pFilepath );
    static const char *GetRomsetName( );
    static int         GetLoadResult( );
    
    // These should only be used for rendering errors regarding missing 
    static RomsetZipArchive *ErrorHandling_GetLoadedRomset_ZipArchives( int *pCount );
    static RomInfo          *ErrorHandling_GetLoadedRoms( int *pCount );
    static int               ErrorHandling_RomRequired( RomInfo *pRom );
    
private:
    static int  ParseRomsetStrings( const char *pFilepath );
    static int  BurnGetDriverIndexByRomsetName( const char *pRomsetName );
    static int  LoadRomInfoForActiveDriver( );
    static int  LoadZipNamesForActiveDriver( );
    static void MapRomsToZipArchives( );
    static int  SearchZipArchiveForRom( RomInfo *pRomInfo, int zipArchiveIndex );
    static void ValidateAllRoms( ValidationResult *pValidationResult );
    
    static int  BurnCallbackFunc_LoadRom( uint8_t *pDest, int *pWrote, int romIndex );

private:
    static RomsetZipArchive mLoadedRomset_ZipArchives[ MAX_ROMSET_ZIPS ];
    static int              mLoadedRomset_ZipArchivesCount;

    static RomInfo          mLoadedRoms[ MAX_ROMS ];
    static int              mLoadedRomsCount;
    
    static char             mLoadedRomset_Path[ MAX_PATH ];
    static char             mLoadedRomset_FileName[ MAX_PATH ];
    static char             mLoadedRomset_Name[ MAX_PATH ];
    
    static int              mRomsetLoaded;
    static int              mRomsetLoadResult;
};

#endif
