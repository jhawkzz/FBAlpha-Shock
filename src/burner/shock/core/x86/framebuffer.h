
// See License.md for license

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

//The path to the frame buffer out on x86 Lubuntu
#define FB_DEVICE "/dev/fb0"

class FrameBuffer
{
public:
        static int  Create( );
        static void Destroy( );
        static void ClearFrameBuffer( );
        static short *GetBackBuffer( );
		
        static void Flip( UINT16 *pBackBuffer, int width, int height );
        
private:
        static void PrintMode( fb_var_screeninfo *pfbVScreenInfo );
        static void PrintVScreenInfo( fb_var_screeninfo *pVScreenInfo );
        static void PrintFixedScreenInfo( fb_fix_screeninfo *pFixedScreenInfo );

private:
        static int  mFrameBufferHandle;
        static int  mFrameBufferBytes;

        static char             *mpFrameBuffer;
        static fb_var_screeninfo mVScreenInfo;
        static fb_fix_screeninfo mFixedScreenInfo;

         char mScaleBuffer[ PLATFORM_LCD_WIDTH * PLATFORM_LCD_HEIGHT * PLATFORM_SCREEN_PITCH ];
};

#endif
