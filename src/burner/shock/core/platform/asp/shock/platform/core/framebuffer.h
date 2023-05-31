
// See License.md for license

#ifndef FRAMEBUFFER_CORE_H_
#define FRAMEBUFFER_CORE_H_

// the amount of bytes from the base frame buffer for the start of the double buffer
#define FB_DOUBLE_BUFFER_OFFSET_BYTES (PLATFORM_LCD_HEIGHT * PLATFORM_SCREEN_PITCH)

//The path to the frame buffer out on the ASP Linux OS
#define FB_DEVICE "/dev/graphics/fb0"

#include "shock/shock.h"

class FrameBufferCore
{
protected:
        static int    MVSXEnableHCD( );
        static void   PrintMode( fb_var_screeninfo *pfbVScreenInfo );
        static void   PrintVScreenInfo( fb_var_screeninfo *pVScreenInfo );
        static void   PrintFixedScreenInfo( fb_fix_screeninfo *pFixedScreenInfo );
        
        static int    mFrameBufferHandle;
        static int    mFrameBufferBytes;
        static char  *mpBackBuffer;
        static char  *mpFrameBuffer;
        static fb_var_screeninfo mVScreenInfo;
};

#endif
