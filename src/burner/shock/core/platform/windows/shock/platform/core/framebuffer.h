
// See License.md for license

#ifndef FRAMEBUFFER_CORE_H_
#define FRAMEBUFFER_CORE_H_

#include "shock/shock.h"

class FrameBufferCore
{
public:
    static void SetWindow(HWND hwnd);
    static void Blit( );

protected:
    static HWND mHwnd;
    static HBITMAP mHbitmap;
    static UINT* mpFrontBuffer;
    static UINT16 mBackBuffer[ FRAMEBUFFER_MAX_WIDTH * FRAMEBUFFER_MAX_HEIGHT ];
    static int mWidth;
    static int mHeight;
};

#endif
