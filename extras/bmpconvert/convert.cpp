
#include <stdio.h>
#include <string.h>
#include "bitmaploader.h"

#define MAX_PATH (256)

int main( int argc, char **argv )
{
    if( argc < 2 )
    {
        printf( "Usage: convert file.bmp\r\n" );
        return 0;
    }
    
    char *pFilename = argv[ 1 ];
    
    BitmapImage bmp;
    BitmapLoader::Load( pFilename, &bmp );
    BitmapLoader::PrintBitmapInfo( &bmp );
    
    BitmapLoader::Make565( &bmp );
    
    char *pLastIndex = strrchr( pFilename, '.' );
    *pLastIndex = 0;
    
    char outFile[ MAX_PATH ] = { 0 };
    snprintf( outFile, MAX_PATH, "%s.h", pFilename );
    
    FILE *pOutFile = fopen( outFile, "wb" );
    
    int imageBytes = bmp.infoHeader.imageSizeBytes;
    
    // note: we write out as short/2 byte to keep the array smaller. This is ok because we only
    // support converting 16bit images.
    unsigned short *pPixelShortData = (unsigned short *)bmp.pPixelData;
    
    fprintf( pOutFile, "UINT16 bmpBytes[%d] = {", imageBytes / 2 );
    for( int i = 0; i < imageBytes / 2; i++ )
    {
        if( i != 0 )
        {
            fprintf( pOutFile, "," );
        }
        
        fprintf( pOutFile, "0x%x", pPixelShortData[ i ] );
        
        if( i % 10 == 0 )
        {
            fprintf( pOutFile, "\r\n" );
        }
    }
    
    // strip off the trailing comma
    fprintf( pOutFile, "};" );
    
    fclose( pOutFile );
    
    // take ANY 3rd argument as confirmation to dump it.
    if( argc > 2 )
    {
        snprintf( outFile, MAX_PATH, "%s-test.bmp", pFilename );
        pOutFile = fopen( outFile, "wb" );
        fwrite( &bmp.fileHeader, 1, sizeof( bmp.fileHeader ), pOutFile );
        fwrite( &bmp.infoHeader, 1, sizeof( bmp.infoHeader ), pOutFile );
        fwrite( bmp.pPixelData, 1, imageBytes, pOutFile );
        fclose( pOutFile );
    }
    
    return 0;
}