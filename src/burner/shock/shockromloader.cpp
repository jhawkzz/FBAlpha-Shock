
// See License.md for license

#include "includes.h"

RomsetZipArchive ShockRomLoader::mLoadedRomset_ZipArchives[ MAX_ROMSET_ZIPS ];
int              ShockRomLoader::mLoadedRomset_ZipArchivesCount;
RomInfo          ShockRomLoader::mLoadedRoms[ MAX_ROMS ];
int              ShockRomLoader::mLoadedRomsCount;
char             ShockRomLoader::mLoadedRomset_Path[ MAX_PATH ];
char             ShockRomLoader::mLoadedRomset_FileName[ MAX_PATH ];
char             ShockRomLoader::mLoadedRomset_Name[ MAX_PATH ];
int              ShockRomLoader::mRomsetLoaded;
int              ShockRomLoader::mRomsetLoadResult;

int ShockRomLoader::LoadRomset( const char *pFilepath )
{
    if( mRomsetLoaded == 1 )
    {
        flushPrintf( "Romset already loaded. Cannot load another!\r\n" );
        return -1;
    }
    
    mRomsetLoadResult = 0;
    
    int result = ShockRomLoader::ParseRomsetStrings( pFilepath );
    if( result == -1 )
    {
        mRomsetLoadResult = LOAD_FATAL_GENERIC;
        flushPrintf( "ShockRomLoader::LoadRomset() - ParseRomsetStrings failed for romset: %s\r\n", pFilepath );
        return -1;
    }
    
    result = ShockRomLoader::BurnGetDriverIndexByRomsetName( ShockRomLoader::mLoadedRomset_Name );
    if( result == -1 )
    {
        mRomsetLoadResult = LOAD_FATAL_UNSUPPORTED;   
        flushPrintf( "ShockRomLoader::LoadRomset() - BurnGetDriverIndexByRomsetName reports romset not found." 
                     "Verify this is a supported romset. Romset: %s\r\n", 
                     ShockRomLoader::mLoadedRomset_Name );
        return -1;
    }
    
    result = ShockRomLoader::LoadRomInfoForActiveDriver( );
    if( result == -1 )
    {
        mRomsetLoadResult = LOAD_FATAL_GENERIC;
        flushPrintf( "ShockRomLoader::LoadRomset() - LoadRomInfoForActiveDriver failed\r\n" );
        return -1;
    }
    
    result = ShockRomLoader::LoadZipNamesForActiveDriver( );
    if( result == -1 )
    {
        mRomsetLoadResult = LOAD_FATAL_GENERIC;
        flushPrintf( "ShockRomLoader::LoadRomset() - LoadZipNamesForActiveDriver failed\r\n" );
        return -1;
    }
    
    ShockRomLoader::MapRomsToZipArchives( );
    
    ValidationResult validationResult = { 0 };
    ShockRomLoader::ValidateAllRoms( &validationResult );
    
    // check the results from validation
    if( validationResult.missingRomError == 1 )
    {
        mRomsetLoadResult = LOAD_FATAL_MISSING_ROM;
        return -1;
    }
    
    if( validationResult.crcError == 1 )
    {
        mRomsetLoadResult |= LOAD_WARNING_CRC;
    }
    
    if( validationResult.lenError == 1 )
    {
        mRomsetLoadResult |= LOAD_WARNING_LEN;
    }
    
    mRomsetLoadResult |= LOAD_SUCCESS;
    
    BurnExtLoadRom = ShockRomLoader::BurnCallbackFunc_LoadRom;
    
    mRomsetLoaded = 1;
    
    return 0;
}

const char *ShockRomLoader::GetRomsetName( )
{
    return mLoadedRomset_Name;
}

int ShockRomLoader::GetLoadResult( )
{
    return mRomsetLoadResult;
}

RomsetZipArchive *ShockRomLoader::ErrorHandling_GetLoadedRomset_ZipArchives( int *pCount )
{
    *pCount = mLoadedRomset_ZipArchivesCount;
    return mLoadedRomset_ZipArchives;
}

RomInfo *ShockRomLoader::ErrorHandling_GetLoadedRoms( int *pCount )
{
    *pCount = mLoadedRomsCount;
    return mLoadedRoms;
}

int ShockRomLoader::ErrorHandling_RomRequired( RomInfo *pRom )
{
    if( pRom->burnRomInfo.nType != 0
        && (pRom->burnRomInfo.nType & BRF_OPT) == 0
        && (pRom->burnRomInfo.nType & BRF_NODUMP) == 0 )
    {
        return 1;
    }
    
    return 0;
}

int ShockRomLoader::ParseRomsetStrings( const char *pFilepath )
{
    // parse the rom name
    const char *pRomsetName = strrchr( pFilepath, '/' );
    if( pRomsetName == NULL )
    {
        flushPrintf( "Failed to parse romset path and file. Filepath received: %s\r\n", pFilepath );
        return -1;
    }
    
    // take the path to the romset, and the romset name itself
    strncpy( mLoadedRomset_Path, pFilepath, pRomsetName - pFilepath + 1 );
    strncpy( mLoadedRomset_FileName, pRomsetName + 1, sizeof( mLoadedRomset_FileName ) );
    
    const char *pExt = strrchr( mLoadedRomset_FileName, '.' );
    if( pExt == NULL )
    {
        flushPrintf( "Failed to parse romset name from file name. Filename received: %s\r\n", mLoadedRomset_FileName );
        return -1;
    }
    strncpy( mLoadedRomset_Name, mLoadedRomset_FileName, pExt - mLoadedRomset_FileName );
    
    // force romsets to lowercase
    strlwr( mLoadedRomset_FileName );
    strlwr( mLoadedRomset_Name );
    
    //flushPrintf( "Loaded Romset Path: %s, FileName: %s, RomsetName: %s\r\n", mLoadedRomset_Path, mLoadedRomset_FileName, mLoadedRomset_Name );
    
    return 0;
}

int ShockRomLoader::BurnGetDriverIndexByRomsetName( const char *pRomsetName )
{
    // go thru every romset FBA supports and find a match by name.
    // as we do this, we'll increment FBA's active driver index, so that
    // when we find a match, its driver index is correctly set.
    // Feels janky, but its what FBA Win32 does.
    int driverIndex = -1;
    
    int i = 0;
    for( i = 0; i < nBurnDrvCount; i++ )
    {
        nBurnDrvActive = i;
        const char *pDriverName = BurnDrvGetText( DRV_NAME );
        
        if( !strcmp( pDriverName, pRomsetName ) )
        {
            driverIndex = nBurnDrvActive;
            break;
        }
    }
    
    // if we failed to find it, reset the driver index.
    if( driverIndex == -1 )
    {
        nBurnDrvActive = ~0U;
    }
    
    return driverIndex;
}

int ShockRomLoader::LoadRomInfoForActiveDriver( )
{
    // given nBurnDrvActive, load the romset data for it.
    memset( mLoadedRoms, 0, sizeof( mLoadedRoms ) );
    mLoadedRomsCount = 0;
    
    while ( !BurnDrvGetRomInfo( &mLoadedRoms[ mLoadedRomsCount ].burnRomInfo, mLoadedRomsCount ) )
    {
        // a rom type of 0 means empty slot in the romlist.
        // (Ex: Neo Geo, AOF specifically, has a lot of this.)
        // we still take it, because our list is a 1:1 copy of FBA's
        if( mLoadedRoms[ mLoadedRomsCount ].burnRomInfo.nType != 0 )
        {
            /*flushPrintf( "romInfo: %s, len %d crc %d type %d count: %d\r\n", &mLoadedRoms[ mLoadedRomsCount ].burnRomInfo.szName,
                    mLoadedRoms[ mLoadedRomsCount ].burnRomInfo.nLen,
                    mLoadedRoms[ mLoadedRomsCount ].burnRomInfo.nCrc,
                    mLoadedRoms[ mLoadedRomsCount ].burnRomInfo.nType, 
                    mLoadedRomsCount );*/
        }
        
        mLoadedRomsCount++;
        
        // this should never, EVER happen. what game are we talking about!?
        if( mLoadedRomsCount >= MAX_ROMS )
        {
            flushPrintf( "ERROR. FBA reports roms in excess of our max support of %d!\r\n", MAX_ROMS );
            return -1;
        }
	}
    
    return 0;
}

int ShockRomLoader::LoadZipNamesForActiveDriver( )
{
    // This will create a priority list of the zip archives needed for the romset.
    //
    // For example, if you're playing X-Men: Children of the Atom's parent version,
    // which is xmcota.zip (the European version), this will get 1 zip file - xmcota.zip.
    //
    // If you're playing the Japanese version, xmcotaj.zip, this will get 2 zip files.
    // xmcotaj.zip and xmcota.zip, in that order.
    //
    // One complication is that if you're loading a nonmerged set, (so like, xmcotaj.zip is not a clone)
    // then it will expect xmcota.zip but it won't need it, so we can't fail just because it doesn't exist.
    //
    // The next function will be responsible for loading the roms in correct priority.
    memset( &mLoadedRomset_ZipArchives, 0, sizeof( mLoadedRomset_ZipArchives ) );
    mLoadedRomset_ZipArchivesCount = 0;
    
    int i = 0;
    for( i = 0; i < MAX_ROMSET_ZIPS; i++ )
    {
        // keep asking FBA for more zips until it tells you there are no more.
        char *pZipName = NULL;
        int result = BurnDrvGetZipName( &pZipName, i );
        if( result != 0 )
        {
            // no more zips
            break;
        }
        
        // create the name and store it in our ziparchives list.
        snprintf( mLoadedRomset_ZipArchives[ mLoadedRomset_ZipArchivesCount ].zipArchiveName, 
                    sizeof( mLoadedRomset_ZipArchives[ mLoadedRomset_ZipArchivesCount ].zipArchiveName ) - 1, 
                    "%s%s", 
                    mLoadedRomset_Path, 
                    pZipName );
                
        if( ZipOpen( mLoadedRomset_ZipArchives[ mLoadedRomset_ZipArchivesCount ].zipArchiveName ) == 0 )
        {
            ZipClose( );
            
            mLoadedRomset_ZipArchives[ mLoadedRomset_ZipArchivesCount ].zipExists = 1;
        }
        
        mLoadedRomset_ZipArchivesCount++;
    }
    
    // now report what exists and what doesn't (debugging)
    //flushPrintf( "FBA Reports Needing %d zip archives\r\n", mLoadedRomset_ZipArchivesCount );
    for( i = 0; i < mLoadedRomset_ZipArchivesCount; i++ )
    {
        if( mLoadedRomset_ZipArchives[ i ].zipExists == 1 )
        {
            //flushPrintf( "%s - Found\r\n", mLoadedRomset_ZipArchives[ i ].zipArchiveName );
        }
        else
        {
            flushPrintf( "%s - WARNING: Not found. Probably ok if loading a nonmerged set.\r\n", 
                            mLoadedRomset_ZipArchives[ i ].zipArchiveName );
        }
    }
    //
    
    // most games have...2? zips. this one somehow had more than 64.
    if( i == MAX_ROMSET_ZIPS )
    {
        flushPrintf( "ERROR! FBA reported more zips available than the supported max of %d\r\n", MAX_ROMSET_ZIPS );
        return -1;
    }
    
    return 0;
}

void ShockRomLoader::MapRomsToZipArchives( )
{
    // we have a list of roms we need, we have a list of zip archives available.
    // go thru each zip archive and look for the rom.
    // remember, the zip archives are ordered in priority. (That's a good thing!)
    int i = 0;
    for( i = 0; i < mLoadedRomset_ZipArchivesCount; i++ )
    {
        // make sure the zip archive was found before we even try and search it
        if( mLoadedRomset_ZipArchives[ i ].zipExists == 1 )
        {
            if ( ZipOpen( mLoadedRomset_ZipArchives[ i ].zipArchiveName ) == 0 )
            {
                int c = 0;
                for( c = 0; c < mLoadedRomsCount; c++ )
                {
                    // if we haven't found it, and it's a valid rom slot, look for it.
                    if( mLoadedRoms[ c ].romFileFound == 0 
                     && mLoadedRoms[ c ].burnRomInfo.nType != 0 )
                    {
                        SearchZipArchiveForRom( &mLoadedRoms[ c ], i );
                    }
                }
            }            
            else
            {
                // This shouldnt ever be possible, but meh
                flushPrintf( "%s - WARNING: Could not open zip archive, tho it was reported as valid.\r\n", 
                    mLoadedRomset_ZipArchives[ i ].zipArchiveName );
            }
        }
    }
}

int ShockRomLoader::SearchZipArchiveForRom( RomInfo *pRomInfo, int zipArchiveIndex )
{
    // this doesn't NEED to be its own function, but its more readable this way.
    ZipEntry *pZipEntryList = NULL;
    int numEntries          = 0;
    
    ZipGetList( &pZipEntryList, &numEntries );
    
    int i = 0;
    for( i = 0; i < numEntries; i++ )
    {
        // if the rom name matches the zip entry, we have a match
        // if the rom crc matches the zip entry, we have a match
        int nameMatch = !stricmp( pZipEntryList[ i ].szName, pRomInfo->burnRomInfo.szName );
        int crcMatch  = pZipEntryList[ i ].nCrc == pRomInfo->burnRomInfo.nCrc;
        if( nameMatch || crcMatch )
        {
            pRomInfo->romFileFound = 1;
            
            // mark the index OF the zip archive in our zip archive list.
            pRomInfo->zipArchiveIndex = zipArchiveIndex;
            
            // mark the index WITHIN the zip archive.
            pRomInfo->indexInZipArchive = i;
            
            pRomInfo->foundCrc = pZipEntryList[ i ].nCrc;
            pRomInfo->foundLen = pZipEntryList[ i ].nLen;
            strncpy( pRomInfo->foundName, 
                     pZipEntryList[ i ].szName, 
                     sizeof( pRomInfo->foundName ) );
            
            // check for problems
            if ( crcMatch == 0 )
            {
                pRomInfo->romCheck |= ROMCHECK_BAD_CRC;
            }
            
            if( nameMatch == 0 )
            {
                pRomInfo->romCheck |= ROMCHECK_NAME_MISMATCH;
            }
            
            // if the zip entry is smaller than what we need, too small
            if( pZipEntryList[ i ].nLen < pRomInfo->burnRomInfo.nLen )
            {
                pRomInfo->romCheck |= ROMCHECK_TOO_SMALL;
            }
            // if the zip entry is larger than what we need, too big
            else if ( pZipEntryList[ i ].nLen > pRomInfo->burnRomInfo.nLen )
            {
                pRomInfo->romCheck |= ROMCHECK_TOO_LARGE;
            }
            
            return 0;
        }
    }
    
    return -1;
}

void ShockRomLoader::ValidateAllRoms( ValidationResult *pValidationResult )
{
    // this acts like a final check to see if we're good to go.
    int i = 0;
    for( i = 0; i < mLoadedRomsCount; i++ )
    {
        if( mLoadedRoms[ i ].burnRomInfo.nType == 0 )
        {
            continue;
        }
        
        // if a rom doesn't exist, we seriously can't continue
        if( mLoadedRoms[ i ].romFileFound == 0 )
        {
            if( (mLoadedRoms[ i ].burnRomInfo.nType & BRF_OPT) == 0 &&
                (mLoadedRoms[ i ].burnRomInfo.nType & BRF_NODUMP) == 0 )
            {
                pValidationResult->missingRomError = 1;
                flushPrintf( "%s - Error, required rom was not found.\r\n", mLoadedRoms[ i ].burnRomInfo.szName );
            }
            else if ( (mLoadedRoms[ i ].burnRomInfo.nType & BRF_OPT) != 0 )
            {
                flushPrintf( "%s - Warning, optional rom was not found.\r\n", mLoadedRoms[ i ].burnRomInfo.szName );
            }
            else if ( (mLoadedRoms[ i ].burnRomInfo.nType & BRF_NODUMP) != 0 )
            {
                flushPrintf( "%s - Warning, nodumped rom was not found.\r\n", mLoadedRoms[ i ].burnRomInfo.szName );
            }
        }
        else if ( mLoadedRoms[ i ].romCheck != 0 )
        {
            if( mLoadedRoms[ i ].romCheck & ROMCHECK_BAD_CRC )
            {
                flushPrintf( "%s - CRC did not match. Expected: 0x%x, Found: 0x%x\r\n", mLoadedRoms[ i ].burnRomInfo.szName,
                                                                                        mLoadedRoms[ i ].burnRomInfo.nCrc,
                                                                                        mLoadedRoms[ i ].foundCrc );
                
                // Note the error, but continue, because depending on settings it isnt fatal.
                pValidationResult->crcError = 1;
            }
            
            if( mLoadedRoms[ i ].romCheck & ROMCHECK_NAME_MISMATCH )
            {
                flushPrintf( "%s - Name did not match. Found: %s. Matched by CRC instead.\r\n", mLoadedRoms[ i ].burnRomInfo.szName,
                                                                                                mLoadedRoms[ i ].foundName );
            }
            
            if( mLoadedRoms[ i ].romCheck & ROMCHECK_TOO_LARGE )
            {
                flushPrintf( "%s - Length did not match. Too large. Expected: %d, Found: %d\r\n", mLoadedRoms[ i ].burnRomInfo.szName,
                                                                                                  mLoadedRoms[ i ].burnRomInfo.nLen,
                                                                                                  mLoadedRoms[ i ].foundLen );
                // Note the error, but continue, because depending on settings it isnt fatal.
                pValidationResult->lenError = 1;
            }
            else if( mLoadedRoms[ i ].romCheck & ROMCHECK_TOO_SMALL )
            {
                flushPrintf( "%s - Length did not match. Too small. Expected: %d, Found: %d\r\n", mLoadedRoms[ i ].burnRomInfo.szName,
                                                                                                  mLoadedRoms[ i ].burnRomInfo.nLen,
                                                                                                  mLoadedRoms[ i ].foundLen );
                                                                                                  
                // Note the error, but continue, because depending on settings it isnt fatal.
                pValidationResult->lenError = 1;
            }
        }
        /*else
        {
            flushPrintf( "%s - Valid! Exists in %s\r\n", mLoadedRoms[ i ].burnRomInfo.szName, 
                                                         mLoadedRomset_ZipArchives[ mLoadedRoms[ i ].zipArchiveIndex ].zipArchiveName );
        }*/
    }
}

int ShockRomLoader::BurnCallbackFunc_LoadRom( uint8_t *pDest, int *pWrote, int romIndex )
{
    if( romIndex < 0 || romIndex >= mLoadedRomsCount )
    {
        flushPrintf( "BurnCallbackFunc_LoadRom() - ERROR - FBA requesting rom index %d, but we our mLoadedRomsCount is %d\r\n", romIndex, mLoadedRomsCount );
        return -1;
    }
    
    int zipIndex = mLoadedRoms[ romIndex ].zipArchiveIndex;
    int result = ZipOpen( mLoadedRomset_ZipArchives[ zipIndex ].zipArchiveName );
    if( result != 0 )
    {
        flushPrintf( "BurnCallbackFunc_LoadRom() - ERROR - Could not open zip archive: %s\r\n", mLoadedRomset_ZipArchives[ zipIndex ].zipArchiveName );
        return -1;
    }
    
    // todo jhm - confirm that we do want foundLen, and not burn info's expected len. That wouldnt work
    // for rom hacks
    result = ZipLoadFile( pDest, mLoadedRoms[ romIndex ].foundLen, pWrote, mLoadedRoms[ romIndex ].indexInZipArchive );
    ZipClose( );
    
    if ( result != 0 )
    {
        flushPrintf( "BurnCallbackFunc_LoadRom() - ERROR - Could not load Rom: %s\r\n"
                      "Zip Archive: %s\r\n"
                      "Rom Len: %d\r\n"
                      "Index in Zip Archive: %d\r\n",
                          mLoadedRomset_ZipArchives[ zipIndex ].zipArchiveName,
                          mLoadedRoms[ romIndex ].foundLen,
                          mLoadedRoms[ romIndex ].indexInZipArchive );
        return -1;
    }

    return 0;
}
