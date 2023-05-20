
// See License.md for license

#ifndef FRAMEBUFFERIMPL_H_
#define FRAMEBUFFERIMPL_H_

#include "../../includes.h"

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
