
// See License.md for license

#ifndef X86FRAMEBUFFER_H_
#define X86FRAMEBUFFER_H_

//The path to the frame buffer out on x86 Lubuntu
#define FB_DEVICE "/dev/fb0"

class X86FrameBuffer
{
public:
		static int  Create( );
		static void Destroy( );
		
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
};

#endif
