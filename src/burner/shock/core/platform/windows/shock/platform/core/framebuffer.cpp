
// See License.md for license

#include "shock/core/framebuffer.h"

HWND FrameBufferCore::mHwnd;
HBITMAP FrameBufferCore::mHbitmap;
UINT* FrameBufferCore::mpFrontBuffer;
UINT16 FrameBufferCore::mBackBuffer[ RESOLUTION_1280_WIDTH * RESOLUTION_1024_HEIGHT ];
int FrameBufferCore::mWidth;
int FrameBufferCore::mHeight;

int FrameBuffer::Create( int width, int height )
{   
    HDC dc = GetDC(mHwnd);
    {
        mWidth = width;
        mHeight = height;

        BITMAPINFO info = {};
        info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info.bmiHeader.biWidth = mWidth;
        info.bmiHeader.biHeight = mHeight;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        mHbitmap = CreateDIBSection(dc, &info, DIB_RGB_COLORS, (void**) &mpFrontBuffer, NULL, 0);

        ReleaseDC(mHwnd, dc);
    }

    return mHbitmap ? 0 : -1;
}

void FrameBuffer::Destroy( )
{
    DeleteObject(mHbitmap);
}

void FrameBuffer::ClearFrameBuffer( )
{
    memset( mBackBuffer, 0, RESOLUTION_1024_HEIGHT * RESOLUTION_1280_WIDTH * FRAMEBUFFER_BYTES_PP );
}

UINT16 *FrameBuffer::GetBackBuffer( )
{
   return (UINT16 *)mBackBuffer;
}

void FrameBuffer::SetSize( int width, int height )
{
    if ( width != mWidth || height != mHeight )
    {
        Destroy( );
        Create( width, height );
    }
}

void FrameBuffer::GetSize( int *pWidth, int *pHeight )
{
    *pWidth = mWidth;
    *pHeight = mHeight;
}

void FrameBuffer::Flip( )
{
    UINT *pFrameBuffer = mpFrontBuffer + (mHeight - 1) * mWidth;
    UINT16 *pScaleBuffer = (UINT16 *)mBackBuffer;

    for( int y = 0; y < mHeight; y++ )
    {
        for( int x = 0; x < mWidth; x++ )
        {
            short pixelData = pScaleBuffer[ x ];

            // break out the components
            short r = (pixelData >> 11) & 0x1F;
            short g = (pixelData >> 5) & 0x3F;
            short b = pixelData & 0x1F;

            // taken from https://stackoverflow.com/questions/8579353/convert-16bit-colour-to-32bit
            int r32 = (r << 3) | (r >> 2);
            int g32 = (g << 2) | (g >> 4); //6bit g
            int b32 = (b << 3) | (b >> 2);

            // its ARGB
            pFrameBuffer[ x ] = 0xFF << 24 | r32 << 16 | g32 << 8 | b32;
        }

        pFrameBuffer -= mWidth;
        pScaleBuffer += mWidth;
    }

    RedrawWindow(mHwnd, NULL, NULL, RDW_UPDATENOW);
}

void FrameBufferCore::SetWindow(HWND hwnd)
{
    mHwnd = hwnd;
}

void FrameBufferCore::Blit()
{
    if (!mHwnd)
        return;

    RECT rect;
    GetClientRect(mHwnd, &rect);

    HDC dc = GetDC(mHwnd);
    HDC hBitmapDC = CreateCompatibleDC(dc);
    HBITMAP oldObj = (HBITMAP) SelectObject(hBitmapDC, mHbitmap); 

    // take the frame buffer and scale it to the max resolution; this simulates what the MVSX / ASP do.
    StretchBlt( dc, 0, 0, RESOLUTION_1280_WIDTH, RESOLUTION_1024_HEIGHT, hBitmapDC, 0, 0, mWidth, mHeight, SRCCOPY );

    SelectObject(hBitmapDC, oldObj); 

    DeleteDC(hBitmapDC);
    ReleaseDC(mHwnd, dc);
}