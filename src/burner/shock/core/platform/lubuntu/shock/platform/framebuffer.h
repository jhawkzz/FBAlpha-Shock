
// See License.md for license

#ifndef FRAMEBUFFER_IMPL_H_
#define FRAMEBUFFER_IMPL_H_

//The path to the frame buffer out on Lubuntu
#define FB_DEVICE "/dev/fb0"

#include "shock/includes.h"

class FrameBufferImpl
{
protected:
        static void PrintMode( fb_var_screeninfo *pfbVScreenInfo );
        static void PrintVScreenInfo( fb_var_screeninfo *pVScreenInfo );
        static void PrintFixedScreenInfo( fb_fix_screeninfo *pFixedScreenInfo );

protected:
        static int  mFrameBufferHandle;
        static int  mFrameBufferBytes;

        static char             *mpFrameBuffer;
        static fb_var_screeninfo mVScreenInfo;
        static fb_fix_screeninfo mFixedScreenInfo;

        static char mScaleBuffer[ PLATFORM_LCD_HEIGHT * PLATFORM_SCREEN_PITCH ];
};

#endif
