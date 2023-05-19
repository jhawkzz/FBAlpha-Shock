
// See License.md for license

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

// the amount of bytes from the base frame buffer for the start of the double buffer
#define FB_DOUBLE_BUFFER_OFFSET_BYTES (PLATFORM_LCD_HEIGHT * PLATFORM_SCREEN_PITCH)

//The path to the frame buffer out on the ASP Linux OS
#define FB_DEVICE "/dev/graphics/fb1"

class FrameBuffer
{
public:
		static int    Create( );
		static void   Destroy( );
		
        static void   ClearFrameBuffer( );
        static short *GetBackBuffer( );
        static void   Flip( );
        
private:
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
