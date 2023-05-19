
// See License.md for license

#include "../../includes.h"

int               FrameBuffer::mFrameBufferHandle;
int               FrameBuffer::mFrameBufferBytes;
char             *FrameBuffer::mpFrameBuffer;
fb_var_screeninfo FrameBuffer::mVScreenInfo;
fb_fix_screeninfo FrameBuffer::mFixedScreenInfo;

char FrameBuffer::mScaleBuffer[ PLATFORM_LCD_HEIGHT * PLATFORM_SCREEN_PITCH ];

int FrameBuffer::Create( )
{    
	mFrameBufferHandle = open( FB_DEVICE, O_RDWR );
	if( mFrameBufferHandle < 0 )
	{
		flushPrintf( "FrameBuffer::Create() Failed to open framebuffer\r\n" );
        
        Destroy( );
		return -1;
	}

    // figure out what the device and driver support
	int result = ioctl( mFrameBufferHandle, FBIOGET_FSCREENINFO, &mFixedScreenInfo );
	if( result == -1 )
    {
		flushPrintf( "FrameBuffer::Create() Error reading screen info for device and driver\r\n" );
        
        Destroy( );
		return -1;
	}
    
    // read the variable screen info so we can configure our preferred color and res
	result = ioctl( mFrameBufferHandle, FBIOGET_VSCREENINFO, &mVScreenInfo );
	if ( result == -1 )
    {
		flushPrintf( "FrameBuffer::Create() Error reading variable screen info\r\n" );
        
        Destroy( );
		return -1;
	}
    
	mFrameBufferBytes = mFixedScreenInfo.smem_len;
    
    // finally map virtual memory to the device so we can write to it.
	mpFrameBuffer = (char *)mmap( NULL, 
                                  mFrameBufferBytes, 
                                  PROT_READ | PROT_WRITE, 
                                  MAP_SHARED, 
                                  mFrameBufferHandle, 
                                  0 );
                          
	if ( mpFrameBuffer == MAP_FAILED ) 
	{
		flushPrintf( "FrameBuffer::Create() Failed to map framebuffer to memory\r\n" );
        
        Destroy( );
		return -1;
	}
    
    // clear the full buffer
    memset( mpFrameBuffer, 0x0, mFrameBufferBytes );
    
    PrintVScreenInfo( &mVScreenInfo );    
    
	return 0;
}

void FrameBuffer::Destroy( )
{
	if( mpFrameBuffer != NULL )
	{
		munmap( mpFrameBuffer, mFrameBufferBytes );
		mpFrameBuffer = NULL;
	}

	if( mFrameBufferHandle > -1 )
	{
		close( mFrameBufferHandle );
		mFrameBufferHandle = -1;
	}

}

void FrameBuffer::ClearFrameBuffer()
{
    memset( mScaleBuffer, 0, PLATFORM_LCD_HEIGHT * PLATFORM_SCREEN_PITCH );
}

short *FrameBuffer::GetBackBuffer( )
{
   return (short *) mScaleBuffer;
}

void FrameBuffer::Flip( )
{
    int *pFrameBuffer = (int *) mpFrameBuffer;
    short *pScaleBuffer = (short *) mScaleBuffer;
    
    for( int y = 0; y < PLATFORM_LCD_HEIGHT; y++ )
    {
        for( int x = 0; x < PLATFORM_LCD_WIDTH; x++ )
        {
            short pixelData = pScaleBuffer[ x ];

            // break out the components
            short r = (pixelData >> 11) & 0x1F;
            short g = (pixelData >> 5) & 0x3F;
            short b = pixelData & 0x1F;
            
            // taken from https://stackoverflow.com/questions/8579353/convert-16bit-colour-to-32bit
            int r32 = (r << 3) | (r >> 2);
            int g32 = (g << 2) | (g >> 4); //6bit g
            int b32 = (b << 3) | (b >> 2);
            
            // its ARGB
            pFrameBuffer[ x ] = 0xFF << 24 | r32 << 16 | g32 << 8 | b32;
        }
        
        pFrameBuffer += mFixedScreenInfo.line_length / 4;
        pScaleBuffer += PLATFORM_LCD_WIDTH;
    }
}

void FrameBuffer::PrintMode( fb_var_screeninfo *pVScreenInfo )
{
    // taken from https://github.com/brgl/busybox/blob/master/util-linux/fbset.c
	double drate = 0, hrate = 0, vrate = 0;

	if (pVScreenInfo->pixclock) {
		drate = 1e12 / pVScreenInfo->pixclock;
		hrate = drate / (pVScreenInfo->left_margin + pVScreenInfo->xres + pVScreenInfo->right_margin + pVScreenInfo->hsync_len);
		vrate = hrate / (pVScreenInfo->upper_margin + pVScreenInfo->yres + pVScreenInfo->lower_margin + pVScreenInfo->vsync_len);
	}
    
    /*
     * D=Dot clock, or pixel rate

        H=Horizontal, or scanline rate

        V=Vertical, or frame rate
    */
	
    flushPrintf("\nmode \"%ux%u-%u\"\n"
        "\t# D: %.3f MHz, H: %.3f kHz, V: %.3f Hz\n"
        "\tgeometry %u %u %u %u %u\n"
        "\ttimings %u %u %u %u %u %u %u\n"
        "\taccel %s\n"
        "\trgba %u/%u,%u/%u,%u/%u,%u/%u\n"
        "endmode\n\n",
            pVScreenInfo->xres, pVScreenInfo->yres, (int) (vrate + 0.5),
            drate / 1e6, hrate / 1e3, vrate,
            pVScreenInfo->xres, pVScreenInfo->yres, pVScreenInfo->xres_virtual, pVScreenInfo->yres_virtual, pVScreenInfo->bits_per_pixel,
            pVScreenInfo->pixclock, pVScreenInfo->left_margin, pVScreenInfo->right_margin, pVScreenInfo->upper_margin, pVScreenInfo->lower_margin,
            pVScreenInfo->hsync_len, pVScreenInfo->vsync_len,
            (pVScreenInfo->accel_flags > 0 ? "true" : "false"),
            pVScreenInfo->red.length, pVScreenInfo->red.offset, pVScreenInfo->green.length, pVScreenInfo->green.offset,
            pVScreenInfo->blue.length, pVScreenInfo->blue.offset, pVScreenInfo->transp.length, pVScreenInfo->transp.offset);
}

void FrameBuffer::PrintVScreenInfo( fb_var_screeninfo *pVScreenInfo )
{
    flushPrintf( "Variable screen info:\r\n"
                    "xres: %d\r\n"
                    "yres: %d\r\n"
                    "xres_virtual: %d\r\n"
                    "yres_virtual: %d\r\n"
                    "xoffset: %d\r\n"
                    "yoffset: %d\r\n"
                    "bits_per_pixel: %d\r\n"
                    "grayscale: %d\r\n"
                    "red: offset: %d, length: %d, msb_right: %d\r\n"
                    "green: offset: %d, length: %d, msb_right: %d\r\n"
                    "blue: offset: %d, length: %d, msb_right: %d\r\n"
                    "transp: offset: %d, length: %d, msb_right: %d\r\n"
                    "nonstd: %d\r\n"
                    "activate: %d\r\n"
                    "height: %d\r\n"
                    "width: %d\r\n"
                    "accel_flags: 0x%x\r\n"
                    "pixclock: %d\r\n"
                    "left_margin: %d\r\n"
                    "right_margin: %d\r\n"
                    "upper_margin: %d\r\n"
                    "lower_margin: %d\r\n"
                    "hsync_len: %d\r\n"
                    "vsync_len: %d\r\n"
                    "sync: %d\r\n"
                    "vmode: %d\r\n"
                    "\r\n", pVScreenInfo->xres, 
                            pVScreenInfo->yres, 
                            pVScreenInfo->xres_virtual,
                            pVScreenInfo->yres_virtual, 
                            pVScreenInfo->xoffset, 
                            pVScreenInfo->yoffset,
                            pVScreenInfo->bits_per_pixel, 
                            pVScreenInfo->grayscale, 
                            pVScreenInfo->red.offset, pVScreenInfo->red.length, pVScreenInfo->red.msb_right, 
                            pVScreenInfo->green.offset, pVScreenInfo->green.length, pVScreenInfo->green.msb_right, 
                            pVScreenInfo->blue.offset, pVScreenInfo->blue.length, pVScreenInfo->blue.msb_right, 
                            pVScreenInfo->transp.offset, pVScreenInfo->transp.length, pVScreenInfo->transp.msb_right, 
                            pVScreenInfo->nonstd, 
                            pVScreenInfo->activate, 
                            pVScreenInfo->height, 
                            pVScreenInfo->width, 
                            pVScreenInfo->accel_flags,
                            pVScreenInfo->pixclock, 
                            pVScreenInfo->left_margin, 
                            pVScreenInfo->right_margin,
                            pVScreenInfo->upper_margin, 
                            pVScreenInfo->lower_margin, 
                            pVScreenInfo->hsync_len,
                            pVScreenInfo->vsync_len, 
                            pVScreenInfo->sync, 
                            pVScreenInfo->vmode );
}

void FrameBuffer::PrintFixedScreenInfo( fb_fix_screeninfo *pFixedScreenInfo )
{
    flushPrintf("Fixed screen info:\r\n"
                    "id: %s\r\n"
                    "smem_start: 0x%lx\r\n"
                    "smem_len: %d\r\n"
                    "type: %d\r\n"
                    "type_aux: %d\r\n"
                    "visual: %d\r\n"
                    "xpanstep: %d\r\n"
                    "ypanstep: %d\r\n"
                    "ywrapstep: %d\r\n"
                    "line_length: %d\r\n"
                    "mmio_start: 0x%lx\r\n"
                    "mmio_len: %d\r\n"
                    "accel: %d\r\n"
                    "\r\n", pFixedScreenInfo->id, 
                            pFixedScreenInfo->smem_start, 
                            pFixedScreenInfo->smem_len, 
                            pFixedScreenInfo->type,
                            pFixedScreenInfo->type_aux, 
                            pFixedScreenInfo->visual,
                            pFixedScreenInfo->xpanstep, 
                            pFixedScreenInfo->ypanstep,
                            pFixedScreenInfo->ywrapstep, 
                            pFixedScreenInfo->line_length, 
                            pFixedScreenInfo->mmio_start,
                            pFixedScreenInfo->mmio_len, 
                            pFixedScreenInfo->accel );
}
