
// See License.md for license

#ifndef BITMAPLOADER_H_
#define BITMAPLOADER_H_

#pragma pack(push,1)
struct BitmapFileHeader 
{
    char  tag[ 2 ];
    int   bmpSize;
    short reserved1;
    short reserved2;
    int   bmpOffset;
};

struct BitmapInfoHeader
{
    int   infoHeaderSize;
    int   width;
    int   height;
    short numColorPlanes;
    short bitsPerPixel;
    int   compressionMethod;
    int   imageSizeBytes; //This is the actual number of pixels in bytes.
    int   imageHorzSize;
    int   imageVertSize;
    int   paletteColors;
    int   numImportantColors;
};

struct BitmapImage
{
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    char            *pPixelData;
};
#pragma pack(pop)

class BitmapLoader
{
public:
    static int Load( const char *pBmpFilePath, BitmapImage *pBmp );
    static int Make565( BitmapImage *pBmp );
    
public:
    static void PrintBitmapInfo( BitmapImage *pBmp );
};

#endif
