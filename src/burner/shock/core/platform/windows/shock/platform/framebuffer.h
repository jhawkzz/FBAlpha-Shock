
// See License.md for license

#ifndef FRAMEBUFFER_CORE_H_
#define FRAMEBUFFER_CORE_H_

#include "shock/includes.h"

class FrameBufferCore
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
