
// See License.md for license

#ifndef FRAMEBUFFER_IMPL_H_
#define FRAMEBUFFER_IMPL_H_

#include "shock/includes.h"

class FrameBufferImpl
{
public:
    static void SetWindow(HWND hwnd);
    static void Blit( );

protected:
    static HWND mHwnd;
    static HBITMAP mHbitmap;
    static UINT* mpFrontBuffer;
    static short mBackBuffer[ PLATFORM_LCD_WIDTH * PLATFORM_LCD_HEIGHT ];
};

#endif
