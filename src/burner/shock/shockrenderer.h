
// See License.md for license

#ifndef SHOCKRENDERER_H_
#define SHOCKRENDERER_H_

#define SCALE_BUFFER_WIDTH (512 * 2)
#define SCALE_BUFFER_HEIGHT (512 * 2)

#include "shock/shock.h"

enum ShockDisplayMode
{
    ShockDisplayMode_Original,
    ShockDisplayMode_AspectRatio,
    ShockDisplayMode_FullScreen,
    ShockDisplayMode_Count
};

class ShockRenderer
{
public:
    static int  Create( );
    static void Destroy( );

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
        int scanLines,
        int smoothing );

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

    static void ScaleKeepAspectRatio( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        int srcPitch,
        UINT16 *pDest,
        int destWidth,
        int destHeight );

    static void ScaleKeepAspectRatio_ScanLine( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        int srcPitch,
        UINT16 *pDest,
        int destWidth,
        int destHeight );

    static void NoScale( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        int srcPitch,
        UINT16 *pDest,
        int destWidth,
        int destHeight );

    static void NoScale_ScanLine( UINT16 *pSource,
        int srcWidth,
        int srcHeight,
        int srcPitch,
        UINT16 *pDest,
        int destWidth,
        int destHeight );

private:

    static UINT16 mRotateBuffer[ 512 * 512 ];
    static UINT16 mScaleBuffer[ SCALE_BUFFER_WIDTH * SCALE_BUFFER_HEIGHT ];
};

#endif
