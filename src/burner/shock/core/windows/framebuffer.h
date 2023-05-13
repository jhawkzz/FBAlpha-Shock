
// See License.md for license

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

class FrameBuffer
{
public:
    static int  Create( );
    static void Destroy( );
    static void ClearFrameBuffer( );
    static short *GetBackBuffer( );

    static void Flip( );

    static void SetWindow(HWND hwnd);
    static void Blit( );

private:
    static HWND mHwnd;
    static HBITMAP mHbitmap;
    static UINT* mpFrontBuffer;
    static short mBackBuffer[ PLATFORM_LCD_WIDTH * PLATFORM_LCD_HEIGHT ];
};

#endif
