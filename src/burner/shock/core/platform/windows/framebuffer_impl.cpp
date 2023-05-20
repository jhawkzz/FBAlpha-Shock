
// See License.md for license

#include "../framebuffer.h"

HWND FrameBufferImpl::mHwnd;
HBITMAP FrameBufferImpl::mHbitmap;
UINT* FrameBufferImpl::mpFrontBuffer;
short FrameBufferImpl::mBackBuffer[ PLATFORM_LCD_WIDTH * PLATFORM_LCD_HEIGHT ];

int FrameBuffer::Create( )
{   
    HDC dc = GetDC(mHwnd);
    {
        BITMAPINFO info = {};
        info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info.bmiHeader.biWidth = PLATFORM_LCD_WIDTH;
        info.bmiHeader.biHeight = PLATFORM_LCD_HEIGHT;
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
    memset( mBackBuffer, 0, PLATFORM_LCD_HEIGHT * PLATFORM_SCREEN_PITCH );
}

short *FrameBuffer::GetBackBuffer( )
{
   return mBackBuffer;
}

void FrameBuffer::Flip( )
{
    UINT *pFrameBuffer = mpFrontBuffer + (PLATFORM_LCD_HEIGHT - 1) * PLATFORM_LCD_WIDTH;
    short *pScaleBuffer = mBackBuffer;

    for( int y = 0; y < PLATFORM_LCD_HEIGHT; y++ )
    {
        for( int x = 0; x < PLATFORM_LCD_WIDTH; x++ )
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

        pFrameBuffer -= PLATFORM_LCD_WIDTH;
        pScaleBuffer += PLATFORM_LCD_WIDTH;
    }

    RedrawWindow(mHwnd, NULL, NULL, RDW_UPDATENOW);
}

void FrameBufferImpl::SetWindow(HWND hwnd)
{
    mHwnd = hwnd;
}

void FrameBufferImpl::Blit()
{
    if (!mHwnd)
        return;

    RECT rect;
    GetClientRect(mHwnd, &rect);

    HDC dc = GetDC(mHwnd);
    HDC hBitmapDC = CreateCompatibleDC(dc);
    HBITMAP oldObj = (HBITMAP) SelectObject(hBitmapDC, mHbitmap); 

    StretchBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hBitmapDC, 0, 0, PLATFORM_LCD_WIDTH, PLATFORM_LCD_HEIGHT, SRCCOPY);

    SelectObject(hBitmapDC, oldObj); 

    DeleteDC(hBitmapDC);
    ReleaseDC(mHwnd, dc);
}