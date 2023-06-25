
// See License.md for license

#ifndef SHOCKRENDERER_H_
#define SHOCKRENDERER_H_

#include "shock/shock.h"

enum ShockDisplayMode
{
    ShockDisplayMode_FullScreen,
    ShockDisplayMode_AspectRatio,
    ShockDisplayMode_Count
};

enum ShockDisplayFilter
{
    ShockDisplayFilter_Pixel,
    ShockDisplayFilter_Pixel_Scanline,
    ShockDisplayFilter_Smoothing,
    ShockDisplayFilter_Performance,
    ShockDisplayFilter_Count
};

class ShockRenderer
{
public:
    static int  Create( int width, int height );
    static void Destroy( );

    static void    SetModeUI( int uiWidth, int uiHeight );
    static void    SetModeFBA( int gameWidth, int gameHeight, int driverFlags );
    static UINT16 *GetBackBuffer( );
    static void    ClearBackBuffer( );
    static void    RenderFBA( UINT16 *pBuffer, int width, int height, int driverFlags, int framesPerSec );
    static void    Flip( );
    static void    CreateThumbnail( UINT16 *pBuffer,
        int width,
        int height,
        UINT16 *pThumbnail,
        int thumbWidth,
        int thumbHeight,
        int driverFlags );

private:
    static void RenderFPS( UINT16 *pBackBuffer, int framesPerSec );

    static void RenderImage( UINT16 *pBackBuffer,
        int width,
        int height,
        UINT16 *pPlatformBackBuffer,
        int platformWidth,
        int platformHeight,
        int driverFlags,
        ShockDisplayMode shockDisplayMode,
        ShockDisplayFilter shockDisplayFilter );

    static void RotateCounterClockwise( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        UINT16 *pDest );

    static void RotateClockwise( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        UINT16 *pDest );

    static void Rotate180( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        UINT16 *pDest );

    static void TwoxSaI_ToDest( UINT16 * pSource,
        int srcWidth,
        int srcHeight,
        int srcPitch,
        UINT16 * pDest,
        int destWidth,
        int destHeight,
        int destPitch );

    static void ScaleToSize( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        int srcPitch,
        UINT16 *pDest,
        int destScaledWidth,
        int destScaledHeight,
        int destRealWidth,
        int destRealHeight );

    static void ScaleToSize_ScanLine( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        int srcPitch,
        UINT16 *pDest,
        int destScaledWidth,
        int destScaledHeight,
        int destRealWidth,
        int destRealHeight );

    static void NoScale( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        int srcPitch,
        UINT16 *pDest,
        int destWidth,
        int destHeight );

private:

    static UINT16 mRotateBuffer[ 512 * 512 ];
};

#endif
