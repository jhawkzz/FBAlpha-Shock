
// See License.md for license

#include "../../includes.h"

HWND FrameBuffer::m_hwnd;
HBITMAP FrameBuffer::m_hbitmap;
UINT* FrameBuffer::m_pFrontBuffer;
short FrameBuffer::m_backBuffer[ PLATFORM_LCD_WIDTH * PLATFORM_LCD_HEIGHT ];

int FrameBuffer::Create( )
{   
    HDC dc = GetDC(m_hwnd);
    {
        BITMAPINFO info = {};
        info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        info.bmiHeader.biWidth = PLATFORM_LCD_WIDTH;
        info.bmiHeader.biHeight = PLATFORM_LCD_HEIGHT;
        info.bmiHeader.biPlanes = 1;
        info.bmiHeader.biBitCount = 32;
        m_hbitmap = CreateDIBSection(dc, &info, DIB_RGB_COLORS, (void**) &m_pFrontBuffer, NULL, 0);

        ReleaseDC(m_hwnd, dc);
    }

    return m_hbitmap ? 0 : -1;
}

void FrameBuffer::Destroy( )
{
    DeleteObject(m_hbitmap);
}

void FrameBuffer::ClearFrameBuffer( )
{
    memset( m_backBuffer, 0, PLATFORM_LCD_HEIGHT * PLATFORM_SCREEN_PITCH );
}

short *FrameBuffer::GetBackBuffer( )
{
   return m_backBuffer;
}

void FrameBuffer::Flip( )
{
    UINT *pFrameBuffer = m_pFrontBuffer + (PLATFORM_LCD_HEIGHT - 1) * PLATFORM_LCD_WIDTH;
    short *pScaleBuffer = m_backBuffer;

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

    RedrawWindow(m_hwnd, NULL, NULL, RDW_UPDATENOW);
}

void FrameBuffer::SetWindow(HWND hwnd)
{
    m_hwnd = hwnd;
}

void FrameBuffer::Blit()
{
    RECT rect;
    GetWindowRect(m_hwnd, &rect);

    HDC dc = GetDC(m_hwnd);
    HDC hBitmapDC = CreateCompatibleDC(dc);
    HBITMAP oldObj = (HBITMAP) SelectObject(hBitmapDC, m_hbitmap); 

    BitBlt(dc, 0, 0, PLATFORM_LCD_WIDTH, PLATFORM_LCD_HEIGHT, hBitmapDC, 0, 0, SRCCOPY);

    SelectObject(hBitmapDC, oldObj); 

    DeleteDC(hBitmapDC);
    ReleaseDC(m_hwnd, dc);
}