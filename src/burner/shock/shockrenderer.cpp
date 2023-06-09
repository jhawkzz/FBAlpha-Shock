
// See License.md for license

#include "shock/core/framebuffer.h"
#include "shock/font/font.h"
#include "shock/shockconfig.h"
#include "shock/shockprofiler.h"
#include "shock/shockrenderer.h"
#include "shock/util/util.h"
#include "shock/shockgame.h"

#include "shock/input/shockinput.h"

UINT16 ShockRenderer::mRotateBuffer[ 512 * 512 ];

int ShockRenderer::Create( int width, int height )
{
    int result = FrameBuffer::Create( width, height );

    if ( result == -1 )
    {
        flushPrintf( "ShockRenderer::Create() - Failed to create platform renderer!\r\n" );
        return -1;
    }

    return 0;
}

void ShockRenderer::Destroy( )
{
    FrameBuffer::Destroy( );
}

void ShockRenderer::SetModeUI( int uiWidth, int uiHeight )
{
    FrameBuffer::SetSize( uiWidth, uiHeight );
    FrameBuffer::ClearFrameBuffer( );
}

void ShockRenderer::SetModeFBA( int gameWidth, int gameHeight, int driverFlags )
{
    // start with the display filter so we know what render path to use.
    switch ( (ShockDisplayFilter)ShockConfig::GetDisplayFilter( ) )
    {
        case ShockDisplayFilter_Pixel:
        case ShockDisplayFilter_Pixel_Scanline:
        {
            if ( ShockDisplayMode_AspectRatio == (ShockDisplayMode)ShockConfig::GetDisplayMode( ) )
            {
                FrameBuffer::SetSize( RESOLUTION_1280_WIDTH / 2, RESOLUTION_1024_HEIGHT / 2 );
            }
            else
            {
                FrameBuffer::SetSize( RESOLUTION_1280_WIDTH, RESOLUTION_720_HEIGHT );
            }
            break;
        }
        
        case ShockDisplayFilter_Smoothing:
        case ShockDisplayFilter_Performance:
        {
            // vertical games need their dimensions flipped
            if ( ( driverFlags & BDF_ORIENTATION_VERTICAL ) )
            {
                // flip width and height for the backbuffer render
                int temp = gameWidth;
                gameWidth = gameHeight;
                gameHeight = temp;
            }

            int frameBufferWidth = 0;
            int frameBufferHeight = 0;

            // for smoothing and performance, the framebuffer
            // should be the size of the game, or a bounding box around it
            // if the display MODE is aspect ratio.
            if ( ShockDisplayMode_AspectRatio == (ShockDisplayMode)ShockConfig::GetDisplayMode( ) )
            {
                // here we want to create a frame buffer that's the aspect ratio of the display
                float srcAspect = (float)gameWidth / gameHeight;
                float destAspect = (float)RESOLUTION_1280_WIDTH / RESOLUTION_1024_HEIGHT;

                // if the game is wider than the device, let it be width dominant
                // and scale height down.
                if ( srcAspect > destAspect )
                {
                    // sf3 is a good example
                    frameBufferWidth = gameWidth;

                    float aspectRatio = (float)RESOLUTION_1024_HEIGHT / (float)RESOLUTION_1280_WIDTH;
                    frameBufferHeight = (int)( (float)frameBufferWidth * aspectRatio );
                }
                // if the game is narrower than the device, let it be height dominant
                // and scale the width UP.
                else
                {
                    // robocop is a good example
                    frameBufferHeight = gameHeight;

                    frameBufferWidth = (int)( (float)frameBufferHeight * destAspect );
                }
            }
            else if ( ShockDisplayMode_FullScreen == (ShockDisplayMode)ShockConfig::GetDisplayMode( ) )
            {
                // for full screen, if the game is vertically oriented, we need to expand the width
                // based on the games aspect ratio, or it'll just look wrong. the taller the game,
                // the more we'll expand the width (which results in a narrower image, because
                // the device scales the entire FB up to full size.)
                float widthScalar = 1.0f;
                if ( (driverFlags & BDF_ORIENTATION_VERTICAL) )
                {
                    float gameAspectRatio = ((float)gameHeight / (float)gameWidth);
                    if ( gameAspectRatio > 1.5f )
                    {
                        widthScalar = 1.22f;
                    }
                    else
                    {
                        widthScalar = 1.1f;
                    }
                }

                frameBufferWidth = (int) ((float)gameWidth * widthScalar);
                frameBufferHeight = gameHeight;
            }
            else if ( ShockDisplayMode_Original2x == (ShockDisplayMode)ShockConfig::GetDisplayMode( ) )
            {
                frameBufferWidth = RESOLUTION_1280_WIDTH / 2;
                frameBufferHeight = RESOLUTION_1024_HEIGHT / 2;
            }

            // finally, if smoothing is on, double the res, since the smoothing algorithm itself
            // does that by nature
            if ( ShockDisplayFilter_Smoothing == (ShockDisplayFilter)ShockConfig::GetDisplayFilter( ) )
            {
                frameBufferWidth *= 2;
                frameBufferHeight *= 2;
            }

            int rem = frameBufferWidth % 4;
            if ( rem != 0 ) frameBufferWidth += 4 - rem;

            rem = frameBufferHeight % 4;
            if ( rem != 0 ) frameBufferHeight += 4 - rem;

            FrameBuffer::SetSize( frameBufferWidth, frameBufferHeight );
            FrameBuffer::ClearFrameBuffer( );

            break;
        }
    }
}

UINT16 *ShockRenderer::GetBackBuffer( )
{
    return (UINT16 *)FrameBuffer::GetBackBuffer( );
}

void ShockRenderer::ClearBackBuffer( )
{
    SHOCK_PROFILE;

    FrameBuffer::ClearFrameBuffer( );
}

void TimerPrintout()
{
    int fbWidth;
    int fbHeight;
    FrameBuffer::GetSize( &fbWidth, &fbHeight );

    Font::SetRenderBuffer( (UINT16 *)FrameBuffer::GetBackBuffer( ), fbWidth, fbHeight );

    struct _context
    {
        int fontWidth;
        int fontHeight;

        int x;
        int y;

        char str[256];
    } context;

    context.fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    context.fontHeight = MET_FONT_LETTER_HEIGHT;
    context.x = 16;//fbWidth - context.fontWidth;
    context.y = 16;//fbHeight - context.fontHeight;

    //flushPrintf("");

    auto cb = [](void* data, scTreeNode<scTimer*>* node)
    {
        _context* c = (_context*) data;

        scTimer* timer = node->val;

        UINT32 i;

        char depth[65];
        for (i = 0; i < node->Depth(); i++)
            depth[i] = ' ';

        depth[i] = 0;
        //flushPrintf("%s%s %dms", depth, timer->Name(), timer->Time() / 1000);

        snprintf(c->str, sizeof(c->str), "%s%s %dms", depth, timer->Name(), timer->Time() / 1000);
        Font::Print( c->str, c->x, c->y, 0xFFFF );

        c->y += c->fontHeight;
    };

    scTimerTree::TraverseDepth(&context, cb);
}

void ShockRenderer::RenderFBA( UINT16 *pBuffer,
    int width,
    int height,
    int driverFlags,
    int framesPerSec )
{
    SHOCK_PROFILE;

    int fbWidth;
    int fbHeight;
    FrameBuffer::GetSize( &fbWidth, &fbHeight );

    RenderImage( pBuffer,
        width,
        height,
        (UINT16 *)FrameBuffer::GetBackBuffer( ),
        fbWidth,
        fbHeight,
        driverFlags,
        (ShockDisplayMode)ShockConfig::GetDisplayMode( ),
        (ShockDisplayFilter)ShockConfig::GetDisplayFilter( )
    );

    if ( ShockConfig::GetShowFPS( ) )
    {
        Font::SetRenderBuffer( (UINT16 *)FrameBuffer::GetBackBuffer( ), fbWidth, fbHeight );
        RenderFPS( (UINT16 *)FrameBuffer::GetBackBuffer( ), framesPerSec );
    }

    //if ( ShockConfig::GetShowTimers( ) )
    {
        TimerPrintout( );
    }
}

void ShockRenderer::Flip( )
{
    SHOCK_PROFILE;

    FrameBuffer::Flip( );
}

void ShockRenderer::RenderFPS( UINT16 *pBackBuffer, int framesPerSec )
{
    SHOCK_PROFILE;

    int fbWidth;
    int fbHeight;
    FrameBuffer::GetSize( &fbWidth, &fbHeight );

    // render a background behind the font so its legible
    int fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    int fontHeight = MET_FONT_LETTER_HEIGHT;

    int fontXPos = fbWidth - fontWidth;
    int fontYPos = fbHeight - fontHeight;

    // account for tv safe frame
    if ( gActivePlatform == ActivePlatform_ASP )
    {
        fontXPos -= 75;
        fontYPos -= 75;
    }

    char fpsStr[ MAX_PATH ];
    snprintf( fpsStr, sizeof( fpsStr ), "%d", framesPerSec );

    pBackBuffer += ( fontYPos * fbWidth ) + fontXPos;

    for ( int y = 0; y < fontHeight; y++ )
    {
        for ( int x = 0; x < fontWidth; x++ )
        {
            pBackBuffer[ x ] = 0;
        }
        pBackBuffer += fbWidth;
    }


    Font::Print( fpsStr, fontXPos, fontYPos, 0xFFFF );
}

void ShockRenderer::CreateThumbnail( UINT16 *pBuffer,
    int width,
    int height,
    UINT16 *pThumbnail,
    int thumbWidth,
    int thumbHeight,
    int driverFlags )
{
    RenderImage( pBuffer, width, height, pThumbnail, thumbWidth, thumbHeight, driverFlags, ShockDisplayMode_FullScreen, ShockDisplayFilter_Pixel );
}

void ShockRenderer::RenderImage( UINT16 *pBackBuffer,
    int width,
    int height,
    UINT16 *pPlatformBackBuffer,
    int platformWidth,
    int platformHeight,
    int driverFlags,
    ShockDisplayMode shockDisplayMode,
    ShockDisplayFilter shockDisplayFilter
    )
{
    SHOCK_PROFILE;

    // these will get set based on driver flags below
    int widthAdjustment = 0;
    UINT16 *pSourceBuffer = NULL;

    if ( ( driverFlags & BDF_ORIENTATION_FLIPPED ) && ( driverFlags & BDF_ORIENTATION_VERTICAL ) )
    {
        // Use Galaxian to test this mode

        RotateClockwise( pBackBuffer, width, height, mRotateBuffer );

        // now flip width and height for the backbufer render
        int temp = width;
        width = height;
        height = temp;

        widthAdjustment = (int) ((float)platformWidth * .1f);

        pSourceBuffer = mRotateBuffer;
    }
    else if ( driverFlags & BDF_ORIENTATION_VERTICAL )
    {
        // Use 1941 to test this mode

        RotateCounterClockwise( pBackBuffer, width, height, mRotateBuffer );

        int temp = width;
        width = height;
        height = temp;

        widthAdjustment = (int) ((float)platformWidth * .2f);

        pSourceBuffer = mRotateBuffer;
    }
    else if ( driverFlags & BDF_ORIENTATION_FLIPPED )
    {
        // Use Rock Climber to test this one

        Rotate180( pBackBuffer, width, height, mRotateBuffer );

        pSourceBuffer = mRotateBuffer;
    }
    // if these are 0, its a traditional right side up image
    else
    {
        pSourceBuffer = pBackBuffer;
    }

    switch ( shockDisplayFilter )
    {
        case ShockDisplayFilter_Performance:
        {
            // just hand the game frame to the driver
            NoScale( (UINT16 *)pSourceBuffer,
                width,
                height,
                width,
                pPlatformBackBuffer,
                platformWidth,
                platformHeight );
            break;
        }

        case ShockDisplayFilter_Smoothing:
        {
            TwoxSaI_ToDest( pSourceBuffer,
                width,
                height,
                width, //we're passing UINT16, so our pitch is width
                pPlatformBackBuffer,
                platformWidth,
                platformHeight,
                platformWidth ); //we're passing UINT16, so our pitch is width
            break;
        }

        case ShockDisplayFilter_Pixel:
        {
            if ( ShockDisplayMode_FullScreen == shockDisplayMode )
            {
                ScaleToSize( (UINT16 *)pSourceBuffer,
                    width,
                    height,
                    width,
                    pPlatformBackBuffer,
                    platformWidth - widthAdjustment,
                    platformHeight,
                    platformWidth,
                    platformHeight );
            }
            else if ( ShockDisplayMode_AspectRatio == shockDisplayMode )
            {
                ScaleKeepAspectRatio( (UINT16 *)pSourceBuffer,
                    width,
                    height,
                    width,
                    pPlatformBackBuffer,
                    platformWidth,
                    platformHeight );
            }
            else if ( ShockDisplayMode_Original2x == shockDisplayMode )
            {
                NoScale( (UINT16 *)pSourceBuffer,
                    width,
                    height,
                    width,
                    pPlatformBackBuffer,
                    platformWidth,
                    platformHeight );
            }
            break;
        }

        case ShockDisplayFilter_Pixel_Scanline:
        {
            if ( ShockDisplayMode_FullScreen == shockDisplayMode )
            {
                ScaleToSize_ScanLine( (UINT16 *)pSourceBuffer,
                    width,
                    height,
                    width,
                    pPlatformBackBuffer,
                    platformWidth - widthAdjustment,
                    platformHeight,
                    platformWidth,
                    platformHeight );
            }
            else if ( ShockDisplayMode_AspectRatio == shockDisplayMode )
            {
                ScaleKeepAspectRatio_ScanLine( (UINT16 *)pSourceBuffer,
                    width,
                    height,
                    width,
                    pPlatformBackBuffer,
                    platformWidth,
                    platformHeight );
            }
            else if ( ShockDisplayMode_Original2x == shockDisplayMode )
            {
                NoScale_ScanLine( (UINT16 *)pSourceBuffer,
                    width,
                    height,
                    width,
                    pPlatformBackBuffer,
                    platformWidth,
                    platformHeight );
            }
            break;
        }
    }
}

void ShockRenderer::RotateCounterClockwise( UINT16 *pSource,
    int srcWidth,
    int srcHeight,
    UINT16 *pDest )
{
    SHOCK_PROFILE;

    int destWidth = srcHeight;
    int destHeight = srcWidth;

    // we'll read the source buffer linearly 
    // we'll write bottom to top, left to right

    // jump the dest pointer to the bottom left corner
    pDest += destWidth * destHeight;

    for ( int destX = 0; destX < destWidth; destX++ )
    {
        int sourceX = 0;
        for ( int destY = 0; destY < destHeight; destY++ )
        {
            // write a pixel, then jump backward one row and write the next pixel
            pDest[ -( destY * destWidth ) ] = pSource[ sourceX++ ];
        }

        // advance to the next column
        pDest++;

        pSource += srcWidth;
    }
}

void ShockRenderer::RotateClockwise( UINT16 *pSource,
    int srcWidth,
    int srcHeight,
    UINT16 *pDest )
{
    SHOCK_PROFILE;

    int destWidth = srcHeight;
    int destHeight = srcWidth;

    // we'll read the source buffer linearly 
    // we'll write top to bottom, right to left

    // jump our dest pointer to the far right corner
    pDest += srcHeight - 1;

    for ( int destX = 0; destX < destWidth; destX++ )
    {
        int sourceX = 0;
        for ( int destY = 0; destY < destHeight; destY++ )
        {
            pDest[ destY * destWidth ] = pSource[ sourceX++ ];
        }

        // move backward one column
        pDest--;

        pSource += srcWidth;
    }
}

void ShockRenderer::Rotate180( UINT16 *pSource,
    int srcWidth,
    int srcHeight,
    UINT16 *pDest )
{
    SHOCK_PROFILE;

    int destWidth = srcWidth;
    int destHeight = srcHeight;

    // we'll read the source buffer linearly 
    // we'll write bottom to top, right to left

    // jump the dest to the bottom left corner
    pDest += destWidth * destHeight;

    for ( int destY = 0; destY < destHeight; destY++ )
    {
        int sourceX = 0;
        for ( int destX = 0; destX < destWidth; destX++ )
        {
            pDest[ srcWidth - 1 - destX ] = pSource[ sourceX++ ];
        }

        // jump back one row
        pDest -= srcWidth;

        pSource += srcWidth;
    }
}

extern "C" {
    extern void _2xSaI( UINT8 *srcPtr, int srcPitch, UINT8 *deltaPtr, UINT8 *dstPtr, int dstPitch, int width, int height );
}

void ShockRenderer::TwoxSaI_ToDest( UINT16 * pSource,
    int srcWidth,
    int srcHeight,
    int srcPitch,
    UINT16 * pDest,
    int destWidth,
    int destHeight,
    int destPitch )
{
    SHOCK_PROFILE;

    // 2xsai by nature will scale the image by a factor of 2 (2x in each dimension)
    // so make sure your buffer fits.

    // when centering, scale up width and height since thats what will actually be rendering)
    int startX = ( destWidth - ( srcWidth * 2 ) ) / 2;
    int startY = ( destHeight - ( srcHeight * 2 ) ) / 2;
    pDest += ( startY * destWidth ) + startX;

    _2xSaI( (UINT8 *)pSource,
        srcPitch * 2, //we're casting to UINT8, so 2x the pitch
        (UINT8 *)pSource,
        (UINT8 *)pDest,
        destPitch * 2, //we're casting to UINT8, so 2x the pitch
        srcWidth,
        srcHeight );
}

void ShockRenderer::ScaleToSize( UINT16 *pSource,
    int srcWidth,
    int srcHeight,
    int srcPitch,
    UINT16 *pDest,
    int destScaledWidth,
    int destScaledHeight,
    int destRealWidth,
    int destRealHeight )
{
    SHOCK_PROFILE;

    // given a source, scale it to dest width/height with no regard for aspect ratio.
    // it will be centered on screen

    int startX = ( destRealWidth - destScaledWidth ) / 2;
    int startY = ( destRealHeight - destScaledHeight ) / 2;

    // calculate the ratio in the high 16 bits so we can do it in whole numbers
    UINT32 xRatio = ( srcWidth << 16 ) / destScaledWidth;
    UINT32 yRatio = ( srcHeight << 16 ) / destScaledHeight;

    UINT16 *pCurrSource = pSource;

    pDest += ( startY * destRealWidth ) + startX;

    int sourceY = 0;
    for ( int destY = 0; destY < destScaledHeight; destY++ )
    {
        int sourceX = 0;
        for ( int destX = 0; destX < destScaledWidth; destX++ )
        {
            pDest[ destX ] = pCurrSource[ sourceX >> 16 ];
            sourceX += xRatio;
        }

        pDest += destRealWidth;

        sourceY += yRatio;
        pCurrSource = pSource + ( ( sourceY >> 16 ) * srcPitch );
    }
}

void ShockRenderer::ScaleToSize_ScanLine( UINT16 *pSource,
    int srcWidth,
    int srcHeight,
    int srcPitch,
    UINT16 *pDest,
    int destScaledWidth,
    int destScaledHeight,
    int destRealWidth,
    int destRealHeight )
{
    SHOCK_PROFILE;

    // given a source, scale it to dest width/height with no regard for aspect ratio.
    // it will be centered on screen

    int startX = ( destRealWidth - destScaledWidth ) / 2;
    int startY = ( destRealHeight - destScaledHeight ) / 2;

    // calculate the ratio in the high 16 bits so we can do it in whole numbers
    UINT32 xRatio = ( srcWidth << 16 ) / destScaledWidth;
    UINT32 yRatio = ( srcHeight << 16 ) / destScaledHeight;

    UINT16 *pCurrSource = pSource;

    pDest += ( startY * destRealWidth ) + startX;

    int sourceY = 0;
    for ( int destY = 0; destY < destScaledHeight; destY++ )
    {
        int sourceX = 0;
        for ( int destX = 0; destX < destScaledWidth; destX++ )
        {
            if ( ( destY % 3 ) == 0 )
            {
                pDest[ destX ] = 0;
            }
            else
            {
                pDest[ destX ] = pCurrSource[ sourceX >> 16 ];
            }
            sourceX += xRatio;
        }

        pDest += destRealWidth;

        sourceY += yRatio;
        pCurrSource = pSource + ( ( sourceY >> 16 ) * srcPitch );
    }
}

void ShockRenderer::ScaleKeepAspectRatio( UINT16 *pSource,
    int srcWidth,
    int srcHeight,
    int srcPitch,
    UINT16 *pDest,
    int destWidth,
    int destHeight )
{
    SHOCK_PROFILE;

    // given a source, scale and center it and maintain aspect ratio.
    int destWidthScaled = destWidth;
    int destHeightScaled = destHeight;

    float srcAspect = (float)srcWidth / srcHeight;
    float destAspect = (float)destWidth / destHeight;

    // if the game is wider than the device, let it be width dominant
    // and scale height down.
    if ( srcAspect > destAspect )
    {
        // sf3 is a good example
        float heightAspect = (float)srcHeight / srcWidth;

        destWidthScaled = destWidth;
        destHeightScaled = destWidth * heightAspect;
    }
    // if the game is narrower than the device, let it be height dominant
    // and scale the width UP.
    else
    {
        // robocop is a good example
        destHeightScaled = destHeight;
        destWidthScaled = destHeight * srcAspect;
    }

    int startX = ( destWidth - destWidthScaled ) / 2;
    int startY = ( destHeight - destHeightScaled ) / 2;

    int xRatio = ( srcWidth << 16 ) / destWidthScaled;
    int yRatio = ( srcHeight << 16 ) / destHeightScaled;

    pDest += ( startY * destWidth ) + startX;

    UINT16 *pCurrSource = pSource;

    int sourceY = 0;
    for ( int destY = 0; destY < destHeightScaled; destY++ )
    {
        int sourceX = 0;
        for ( int destX = 0; destX < destWidthScaled; destX++ )
        {
            pDest[ destX ] = pCurrSource[ (int)sourceX >> 16 ];
            sourceX += xRatio;
        }

        pDest += destWidth;

        sourceY += yRatio;
        pCurrSource = pSource + ( ( sourceY >> 16 ) * srcPitch );
    }
}

void ShockRenderer::ScaleKeepAspectRatio_ScanLine( UINT16 *pSource,
    int srcWidth,
    int srcHeight,
    int srcPitch,
    UINT16 *pDest,
    int destWidth,
    int destHeight )
{
    SHOCK_PROFILE;

    // given a source, scale and center it and maintain aspect ratio.
    int destWidthScaled = destWidth;
    int destHeightScaled = destHeight;

    float srcAspect = (float)srcWidth / srcHeight;
    float destAspect = (float)destWidth / destHeight;

    // if the game is wider than the device, let it be width dominant
    // and scale height down.
    if ( srcAspect > destAspect )
    {
        // sf3 is a good example
        float heightAspect = (float)srcHeight / srcWidth;

        destWidthScaled = destWidth;
        destHeightScaled = destWidth * heightAspect;
    }
    // if the game is narrower than the device, let it be height dominant
    // and scale the width UP.
    else
    {
        // robocop is a good example
        destHeightScaled = destHeight;
        destWidthScaled = destHeight * srcAspect;
    }

    int startX = ( destWidth - destWidthScaled ) / 2;
    int startY = ( destHeight - destHeightScaled ) / 2;

    int xRatio = ( srcWidth << 16 ) / destWidthScaled;
    int yRatio = ( srcHeight << 16 ) / destHeightScaled;

    pDest += ( startY * destWidth ) + startX;

    UINT16 *pCurrSource = pSource;

    int sourceY = 0;
    for ( int destY = 0; destY < destHeightScaled; destY++ )
    {
        int sourceX = 0;
        for ( int destX = 0; destX < destWidthScaled; destX++ )
        {
            if ( ( destY % 3 ) == 0 )
            {
                pDest[ destX ] = 0;
            }
            else
            {
                pDest[ destX ] = pCurrSource[ (int)sourceX >> 16 ];
            }
            sourceX += xRatio;
        }

        pDest += destWidth;

        sourceY += yRatio;
        pCurrSource = pSource + ( ( sourceY >> 16 ) * srcPitch );
    }
}

void ShockRenderer::NoScale( UINT16 *pSource,
    int srcWidth,
    int srcHeight,
    int srcPitch,
    UINT16 *pDest,
    int destWidth,
    int destHeight )
{
    SHOCK_PROFILE;
    // just a simple copy

    int startX = ( destWidth - srcWidth ) / 2;
    int startY = ( destHeight - srcHeight ) / 2;

    pDest += ( startY * destWidth ) + startX;

    for ( int y = 0; y < srcHeight; y++ )
    {
        for ( int x = 0; x < srcWidth; x++ )
        {
            pDest[ x ] = pSource[ x ];
        }

        pDest += destWidth;
        pSource += srcPitch;
    }
}

void ShockRenderer::NoScale_ScanLine( UINT16 *pSource,
    int srcWidth,
    int srcHeight,
    int srcPitch,
    UINT16 *pDest,
    int destWidth,
    int destHeight )
{
    SHOCK_PROFILE;
    // just a simple copy

    int startX = ( destWidth - srcWidth ) / 2;
    int startY = ( destHeight - srcHeight ) / 2;

    pDest += ( startY * destWidth ) + startX;

    for ( int y = 0; y < srcHeight; y++ )
    {
        for ( int x = 0; x < srcWidth; x++ )
        {
            if ( ( y % 3 ) == 0 )
            {
                pDest[ x ] = 0;
            }
            else
            {
                pDest[ x ] = pSource[ x ];
            }
        }

        pDest += destWidth;
        pSource += srcPitch;
    }
}
