
// See License.md for license

#include "includes.h"

UINT16 ShockRenderer::mRotateBuffer[ ROTATE_BUFFER_WIDTH * ROTATE_BUFFER_HEIGHT ];

int ShockRenderer::Create( )
{
    int result = FrameBuffer::Create( );

    if( result == -1 )
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

UINT16 *ShockRenderer::GetBackBuffer( )
{
    return (UINT16 *)FrameBuffer::GetBackBuffer( );
}

void ShockRenderer::ClearBackBuffer( )
{
    FrameBuffer::ClearFrameBuffer( );
}

void ShockRenderer::RenderFBA( UINT16 *pBuffer, 
                               int width, 
                               int height, 
                               int driverFlags, 
                               int framesPerSec )
{
    RenderImage( pBuffer, width, height, (UINT16 *)FrameBuffer::GetBackBuffer(), PLATFORM_LCD_WIDTH, PLATFORM_LCD_HEIGHT, driverFlags );
    
    if( ShockConfig::GetShowFPS( ) )
    {
        RenderFPS( (UINT16 *)FrameBuffer::GetBackBuffer(), framesPerSec );
    }
}

void ShockRenderer::Flip( )
{
    FrameBuffer::Flip( );
}

void ShockRenderer::RenderFPS( UINT16 *pBackBuffer, int framesPerSec )
{
    // render a background behind the font so its legible
    int fontWidth = MET_FONT_LETTER_WIDTH * 2 + FONT_SPACING;
    
    char fpsStr[ MAX_PATH ];
    snprintf( fpsStr, sizeof( fpsStr ), "%d", framesPerSec );
    
    for( int y = PLATFORM_LCD_HEIGHT - MET_FONT_LETTER_HEIGHT; y < PLATFORM_LCD_HEIGHT; y++ )
    {
        for( int x = PLATFORM_LCD_WIDTH - fontWidth; x < PLATFORM_LCD_WIDTH; x++ )
        {
            pBackBuffer[ y * PLATFORM_LCD_WIDTH + x ] = 0;
        }
    }
    
    Font::Print( pBackBuffer, fpsStr, PLATFORM_LCD_WIDTH - fontWidth, PLATFORM_LCD_HEIGHT - MET_FONT_LETTER_HEIGHT, 0xFFFF );
}

void ShockRenderer::RenderImage( UINT16 *pBackBuffer, 
                                 int width, 
                                 int height, 
                                 UINT16 *pPlatformBackBuffer, 
                                 int platformWidth, 
                                 int platformHeight,
                                 int driverFlags )
{
    // these will get set based on driver flags below
    int widthAdjustment = 0;
    UINT16 *pSourceBuffer = NULL;
    
    if ( (driverFlags & BDF_ORIENTATION_FLIPPED) && (driverFlags & BDF_ORIENTATION_VERTICAL) )
    {
        // Use Galaxian to test this mode
        
        RotateClockwise( pBackBuffer, width, height, mRotateBuffer );
        
        // now flip width and height for the backbufer render
        int temp = width;
        width    = height;
        height   = temp;
        
        widthAdjustment = 128;
        
        pSourceBuffer = mRotateBuffer;
    }
    else if ( driverFlags & BDF_ORIENTATION_VERTICAL )
    {
        // Use 1941 to test this mode
        
        RotateCounterClockwise( pBackBuffer, width, height, mRotateBuffer );
        
        int temp = width;
        width    = height;
        height   = temp;
        
        widthAdjustment = 256;
        
        pSourceBuffer = mRotateBuffer;
    }
    else if( driverFlags & BDF_ORIENTATION_FLIPPED )
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
    
    // now figure out how to render to the backbuffer
    switch( (ShockDisplayMode)ShockConfig::GetDisplayMode( ) )
    {
        case ShockDisplayMode_FullScreen:
        {
            if( ShockConfig::GetScanLinesEnabled() )
            {
                ScaleToSize_ScanLine( (UINT16 *)pSourceBuffer, 
                                      width, 
                                      height, 
                                      pPlatformBackBuffer, 
                                      platformWidth - widthAdjustment, 
                                      platformHeight, 
                                      platformWidth, 
                                      platformHeight );
            }
            else
            {
                ScaleToSize( (UINT16 *)pSourceBuffer, 
                              width, 
                              height, 
                              pPlatformBackBuffer, 
                              platformWidth - widthAdjustment, 
                              platformHeight, 
                              platformWidth, 
                              platformHeight );
            }
            break;
        }
        
        case ShockDisplayMode_AspectRatio:
        {
            if( ShockConfig::GetScanLinesEnabled() )
            {
                ScaleKeepAspectRatio_ScanLine( (UINT16 *)pSourceBuffer, 
                                               width, 
                                               height, 
                                               pPlatformBackBuffer, 
                                               platformWidth, 
                                               platformHeight );
            }
            else
            {
                ScaleKeepAspectRatio( (UINT16 *)pSourceBuffer, 
                                      width, 
                                      height, 
                                      pPlatformBackBuffer, 
                                      platformWidth, 
                                      platformHeight );
            }
            break;
        }
        
        case ShockDisplayMode_Original:
        {
            if( ShockConfig::GetScanLinesEnabled() )
            {
                NoScale_ScanLine( (UINT16 *)pSourceBuffer, 
                                  width, 
                                  height, 
                                  pPlatformBackBuffer, 
                                  platformWidth, 
                                  platformHeight );
            }
            else
            {
                NoScale( (UINT16 *)pSourceBuffer, 
                         width, 
                         height, 
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
    int destWidth = srcHeight;
    int destHeight = srcWidth;
    
    // we'll read the source buffer linearly 
    // we'll write bottom to top, left to right
    
    // jump the dest pointer to the bottom left corner
    pDest += destWidth * destHeight;
    
    for( int destX = 0; destX < destWidth; destX++ )
    {
        int sourceX = 0;
        for( int destY = 0; destY < destHeight; destY++ )
        {
            // write a pixel, then jump backward one row and write the next pixel
            pDest[ -(destY * destWidth) ] = pSource[ sourceX++ ];
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
    int destWidth = srcHeight;
    int destHeight = srcWidth;
    
    // we'll read the source buffer linearly 
    // we'll write top to bottom, right to left
    
    // jump our dest pointer to the far right corner
    pDest += srcHeight - 1;
    
    for( int destX = 0; destX < destWidth; destX++ )
    {
        int sourceX = 0;
        for( int destY = 0; destY < destHeight; destY++ )
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
    int destWidth = srcWidth;
    int destHeight = srcHeight;
    
    // we'll read the source buffer linearly 
    // we'll write bottom to top, right to left
    
    // jump the dest to the bottom left corner
    pDest += destWidth * destHeight;
    
    for( int destY = 0; destY < destHeight; destY++ )
    {
        int sourceX = 0;
        for( int destX = 0; destX < destWidth; destX++ )
        {
            pDest[ srcWidth - 1 - destX ] = pSource[ sourceX++ ];
        }

        // jump back one row
        pDest -= srcWidth;
        
        pSource += srcWidth;
    }
}

void ShockRenderer::ScaleToSize( UINT16 *pSource, 
                                 int srcWidth, 
                                 int srcHeight,
                                 UINT16 *pDest,
                                 int destScaledWidth,
                                 int destScaledHeight,
                                 int destRealWidth,
                                 int destRealHeight )
{
    // given a source, scale it to dest width/height with no regard for aspect ratio.
    // it will be centered on screen
        
    int startX = (destRealWidth - destScaledWidth) / 2;
    int startY = (destRealHeight - destScaledHeight) / 2;
    
    // calculate the ratio in the high 16 bits so we can do it in whole numbers
    UINT32 xRatio = ( srcWidth << 16 ) / destScaledWidth;
    UINT32 yRatio = ( srcHeight << 16 ) / destScaledHeight;
    
    UINT16 *pCurrSource = pSource;
    
    pDest += (startY * destRealWidth) + startX;
    
    int sourceY = 0;
    for( int destY = 0; destY < destScaledHeight; destY++ )
    {
        int sourceX = 0;   
        for( int destX = 0; destX < destScaledWidth; destX++ )
        {
            pDest[ destX ] = pCurrSource[ sourceX >> 16 ];
            sourceX += xRatio;
        }
        
        pDest += destRealWidth;
        
        sourceY += yRatio;
        pCurrSource = pSource + ((sourceY >> 16) * srcWidth);
    }   
}

void ShockRenderer::ScaleToSize_ScanLine( UINT16 *pSource, 
                                          int srcWidth, 
                                          int srcHeight,
                                          UINT16 *pDest,
                                          int destScaledWidth,
                                          int destScaledHeight,
                                          int destRealWidth,
                                          int destRealHeight )
{
    // given a source, scale it to dest width/height with no regard for aspect ratio.
    // it will be centered on screen
        
    int startX = (destRealWidth - destScaledWidth) / 2;
    int startY = (destRealHeight - destScaledHeight) / 2;
    
    // calculate the ratio in the high 16 bits so we can do it in whole numbers
    UINT32 xRatio = ( srcWidth << 16 ) / destScaledWidth;
    UINT32 yRatio = ( srcHeight << 16 ) / destScaledHeight;
    
    UINT16 *pCurrSource = pSource;
    
    pDest += (startY * destRealWidth) + startX;
        
    int sourceY = 0;
    for( int destY = 0; destY < destScaledHeight; destY++ )
    {
        int sourceX = 0;   
        for( int destX = 0; destX < destScaledWidth; destX++ )
        {
            if( (destY % 3) == 0 )
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
        pCurrSource = pSource + ((sourceY >> 16) * srcWidth);
    }   
}

void ShockRenderer::ScaleKeepAspectRatio( UINT16 *pSource, 
                                          int srcWidth, 
                                          int srcHeight,
                                          UINT16 *pDest,
                                          int destWidth,
                                          int destHeight )
{
    // given a source, scale and center it and maintain aspect ratio.
    int destWidthScaled  = destWidth;
    int destHeightScaled = destHeight;
    
    if( srcWidth > srcHeight )
    {
        float aspectRatio = (float)srcHeight / srcWidth;
        destHeightScaled = destWidth * aspectRatio;
        
        // because our display is wider than it is tall,
        // do one additional check to ensure the height
        // ratio allowed it to be <= the height of the display.
        if( destHeightScaled >= destHeight )
        {
            // clamp the width to the height of the screen,
            // and then scale height down by that.
            destWidthScaled = destHeight;
            destHeightScaled = destWidthScaled * aspectRatio;
        }
    }
    else
    {
        float aspectRatio = (float)srcWidth / srcHeight;
        destWidthScaled = destHeight * aspectRatio;
    }
    
    int startX = (destWidth - destWidthScaled) / 2;
    int startY = (destHeight - destHeightScaled) / 2;
    
    int xRatio = (srcWidth << 16)  / destWidthScaled;
    int yRatio = (srcHeight << 16) / destHeightScaled;
  
    pDest += (startY * destWidth) + startX;
    
    UINT16 *pCurrSource = pSource;
    
    int sourceY = 0;
    for( int destY = 0; destY < destHeightScaled; destY++ )
    {
        int sourceX = 0;
        for( int destX = 0; destX < destWidthScaled; destX++ )
        {
            pDest[ destX ] = pCurrSource[ (int)sourceX >> 16 ];
            sourceX += xRatio;
        }
        
        pDest += destWidth;
        
        sourceY += yRatio;
        pCurrSource = pSource + ((sourceY >> 16) * srcWidth);
    }
}

void ShockRenderer::ScaleKeepAspectRatio_ScanLine( UINT16 *pSource, 
                                                   int srcWidth, 
                                                   int srcHeight,
                                                   UINT16 *pDest,
                                                   int destWidth,
                                                   int destHeight )
{
    // given a source, scale and center it and maintain aspect ratio.
    
    int destWidthScaled  = destWidth;
    int destHeightScaled = destHeight;
    
    if( srcWidth > srcHeight )
    {
        float aspectRatio = (float)srcHeight / srcWidth;
        destHeightScaled = destWidth * aspectRatio;
        
        // because our display is wider than it is tall,
        // do one additional check to ensure the height
        // ratio allowed it to be <= the height of the display.
        if( destHeightScaled >= destHeight )
        {
            // clamp the width to the height of the screen,
            // and then scale height down by that.
            destWidthScaled = destHeight;
            destHeightScaled = destWidthScaled * aspectRatio;
        }
    }
    else
    {
        float aspectRatio = (float)srcWidth / srcHeight;
        destWidthScaled = destHeight * aspectRatio;
    }
    
    int startX = (destWidth - destWidthScaled) / 2;
    int startY = (destHeight - destHeightScaled) / 2;
    
    int xRatio = (srcWidth << 16)  / destWidthScaled;
    int yRatio = (srcHeight << 16) / destHeightScaled;
  
    pDest += (startY * destWidth) + startX;
    
    UINT16 *pCurrSource = pSource;
    
    int sourceY = 0;
    for( int destY = 0; destY < destHeightScaled; destY++ )
    {
        int sourceX = 0;
        for( int destX = 0; destX < destWidthScaled; destX++ )
        {
            if( (destY % 3) == 0 )
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
        pCurrSource = pSource + ((sourceY >> 16) * srcWidth);
    }
}

void ShockRenderer::NoScale( UINT16 *pSource, 
                             int srcWidth, 
                             int srcHeight,
                             UINT16 *pDest,
                             int destWidth,
                             int destHeight )
{
    // just a simple copy
    
    int startX = (destWidth - srcWidth) / 2;
    int startY = (destHeight - srcHeight) / 2;
    
    pDest += (startY * destWidth) + startX;
    
    for( int y = 0; y < srcHeight; y++ )
    {
        for( int x = 0; x < srcWidth; x++ )
        {
            pDest[ x ] = pSource[ x ];
        }
        
        pDest += destWidth;
        pSource += srcWidth;
    }
}

void ShockRenderer::NoScale_ScanLine( UINT16 *pSource, 
                                      int srcWidth, 
                                      int srcHeight,
                                      UINT16 *pDest,
                                      int destWidth,
                                      int destHeight )
{
    // just a simple copy
    
    int startX = (destWidth - srcWidth) / 2;
    int startY = (destHeight - srcHeight) / 2;
    
    pDest += (startY * destWidth) + startX;
    
    for( int y = 0; y < srcHeight; y++ )
    {
        for( int x = 0; x < srcWidth; x++ )
        {
            if( (y % 3) == 0 )
            {
                pDest[ x ] = 0;
            }
            else
            {
                pDest[ x ] = pSource[ x ];
            }
        }
        
        pDest += destWidth;
        pSource += srcWidth;
    }
}
