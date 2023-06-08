
// See License.md for license

#ifndef FRAMEBUFFER_CORE_H_
#define FRAMEBUFFER_CORE_H_

#include "shock/systems.h"


//The path to the frame buffer out on the MVSX Linux OS
#define FB_DEVICE "/dev/graphics/fb0"

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
