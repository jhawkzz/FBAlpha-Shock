
// See License.md for license

#include "bitmaploader.h"
#include <stdio.h>
#include <memory.h>

int BitmapLoader::Load( const char *pBmpFilePath, BitmapImage *pBmp )
{
    FILE *pBmpFile = fopen( pBmpFilePath, "rb" );
    if( pBmpFile == NULL )        
    {
        printf( "BitmapLoader::Load() Failed to open file: %s\r\n", pBmpFilePath );
        return -1;
    }
    
    int sizeRead = fread( &pBmp->fileHeader, 1, sizeof( BitmapFileHeader ), pBmpFile );
    if ( sizeRead != sizeof( BitmapFileHeader ) )
    {
        printf( "BitmapLoader::Load() Error reading BitmapFileHeader. Expected %d bytes. Read %d bytes.\r\n", 
                        sizeof( BitmapFileHeader ), 
                        sizeRead );
        return -1;
    }
    
    if( pBmp->fileHeader.tag[ 0 ] != 'B' || pBmp->fileHeader.tag[ 1 ] != 'M' )
    {
        printf( "BitmapLoader::Load() Invalid BMP Header Tag. Expected 'BM'. Read '%s'\r\n", pBmp->fileHeader.tag );
        return -1;
    }
    
    
    
    sizeRead = fread( &pBmp->infoHeader, 1, sizeof( BitmapInfoHeader ), pBmpFile );
    if ( sizeRead != sizeof( BitmapInfoHeader ) )
    {
         printf( "BitmapLoader::Load() Error reading BitmapInfoHeader. Expected %d bytes. Read %d bytes.\r\n", 
                        sizeof( BitmapInfoHeader ), 
                        sizeRead );
        return -1;
    }
    
    if( pBmp->infoHeader.compressionMethod != 0 )
    {
        printf( "BitmapLoader::Load() Compression method is not 0. Found: %d. Currently no compression is supported.\r\n", 
                      pBmp->infoHeader.compressionMethod );
                      
        return -1;
    }
    
    pBmp->pPixelData = new char[ pBmp->infoHeader.imageSizeBytes ];
    
    // gotta read bottom up
    if( pBmp->infoHeader.height > 0 )
    {
        char *pScratchBuffer = new char[ pBmp->infoHeader.imageSizeBytes ];
        sizeRead = 0;
        
        int bmpPitch = pBmp->infoHeader.width * 2;
        
        // get our stride
        int bytesPerPixel = (pBmp->infoHeader.bitsPerPixel + 7) / 8;
        int bmpStride = 4 * ((pBmp->infoHeader.width * bytesPerPixel + 3) / 4);
        
        // position the buffer at the start of the last row
        char *pCurrPixelData = pBmp->pPixelData;
        
        sizeRead = fread( pScratchBuffer, 1, pBmp->infoHeader.imageSizeBytes, pBmpFile );
        
        char *pCurrScratchBuffer = pScratchBuffer + (bmpStride * pBmp->infoHeader.height) - bmpStride;
        
        // read each row and write it bottom-up to the buffer
        int y = 0;
        for( y = 0; y < pBmp->infoHeader.height; y++ )
        {
            for( int x = 0; x < bmpPitch; x++ )
            {
                pCurrPixelData[ x ] = pCurrScratchBuffer[ x ];
            }
            
            pCurrPixelData += bmpStride;
            pCurrScratchBuffer -= bmpStride;
        }
        
        delete [] pScratchBuffer;
    }
    else
    {
        sizeRead = fread( pBmp->pPixelData, 1, pBmp->infoHeader.imageSizeBytes, pBmpFile );
    }
    
    if( sizeRead != pBmp->infoHeader.imageSizeBytes )
    {
        printf( "BitmapLoader::Load() Error reading bitmap pixels! Expected: %d bytes. Read %d bytes.\r\n", pBmp->infoHeader.imageSizeBytes, sizeRead );
        return -1;
    }
    
    fclose( pBmpFile );
    pBmpFile = NULL;
    
    return 0;
}

int BitmapLoader::Make565( BitmapImage *pBmp )
{
    if( pBmp->infoHeader.bitsPerPixel != 16 )
    {
        printf( "BitmapLoader::Make565() Bitmap is not 16 bit. BPP is: %d\r\n", pBmp->infoHeader.bitsPerPixel );
        return -1;
    }
    
    short *pPixelBuffer = (short *)pBmp->pPixelData;
    
    int y = 0;
    for( y = 0; y < pBmp->infoHeader.height; y++ )
    {
        int x = 0;
        for( x = 0; x < pBmp->infoHeader.width; x++ )
        {
            short pixelData = pPixelBuffer[ x ];
 
            // break out the components
            int r = (pixelData & (0x1F << 10)) >> 10;
            int g = (pixelData & (0x1F << 5)) >> 5;
            int b = pixelData & 0x1F;
            
            // scale to six bits
            int scaledG = (int) (((float)g / (float)0x1F) * 0x3F);
          
            short pixelData565 = r << 11 | scaledG << 5 | b; 
            pPixelBuffer[ x ] = pixelData565;
        }
        
        pPixelBuffer += pBmp->infoHeader.width;
    }
    
    return 0;
}

void BitmapLoader::PrintBitmapInfo( BitmapImage *pBmp )
{
    // header
    printf( "BitmapLoader::Load() bmp header struct is: %d\r\n", sizeof( pBmp->fileHeader ) );
    printf( "BitmapLoader::Load() bmp tag is: %c%c\r\n", pBmp->fileHeader.tag[0], pBmp->fileHeader.tag[1] );
    printf( "BitmapLoader::Load() bmp size is: %d\r\n", pBmp->fileHeader.bmpSize );
    printf( "BitmapLoader::Load() reserved 1 & 2 are: %d %d\r\n", pBmp->fileHeader.reserved1, pBmp->fileHeader.reserved2 );
    printf( "BitmapLoader::Load() bmp offset is: %d\r\n", pBmp->fileHeader.bmpOffset );
    
    // info header
    printf( "BitmapLoader::Load() infoHeaderSize reports %d actual struct: %d\r\n", pBmp->infoHeader.infoHeaderSize, sizeof( pBmp->infoHeader ) );
    printf( "BitmapLoader::Load() width %d\r\n", pBmp->infoHeader.width );
    printf( "BitmapLoader::Load() height %d\r\n", pBmp->infoHeader.height );
    printf( "BitmapLoader::Load() numColorPlanes %d\r\n", pBmp->infoHeader.numColorPlanes );
    printf( "BitmapLoader::Load() bitsPerPixel %d\r\n", pBmp->infoHeader.bitsPerPixel );
    printf( "BitmapLoader::Load() compressionMethod %d\r\n", pBmp->infoHeader.compressionMethod );
    printf( "BitmapLoader::Load() imageSizeBytes %d\r\n", pBmp->infoHeader.imageSizeBytes );
    printf( "BitmapLoader::Load() imageHorzSize %d\r\n", pBmp->infoHeader.imageHorzSize );
    printf( "BitmapLoader::Load() imageVertSize %d\r\n", pBmp->infoHeader.imageVertSize );
    printf( "BitmapLoader::Load() paletteColors %d\r\n", pBmp->infoHeader.paletteColors );
    printf( "BitmapLoader::Load() numImportantColors %d\r\n", pBmp->infoHeader.numImportantColors );
    printf( "\r\n" );
}
