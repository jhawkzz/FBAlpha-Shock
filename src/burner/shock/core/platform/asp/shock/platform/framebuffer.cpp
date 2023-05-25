
// See License.md for license

#include "shock/core/framebuffer.h"

int   FrameBufferCore::mFrameBufferHandle;
int   FrameBufferCore::mFrameBufferBytes;
char *FrameBufferCore::mpFrameBuffer;
char *FrameBufferCore::mpBackBuffer;
fb_var_screeninfo FrameBufferCore::mVScreenInfo;

int FrameBuffer::Create( )
{
    int hcdResult = MVSXEnableHCD( );
    if ( hcdResult == -1 )
    {
        flushPrintf( "WARNING: FrameBuffer::Create() call to MVSXEnableHCD() failed. Not fatal? Will continue.\r\n" );
    }
    
	mFrameBufferHandle = open( FB_DEVICE, O_RDWR );
	if( mFrameBufferHandle < 0 )
	{
		flushPrintf( "FrameBuffer::Create() Failed to open framebuffer\r\n" );
        
        Destroy( );
		return -1;
	}

    // figure out what the device and driver support
    fb_fix_screeninfo fixedScreenInfo;
	int result = ioctl( mFrameBufferHandle, FBIOGET_FSCREENINFO, &fixedScreenInfo );
	if( result == -1 )
    {
		flushPrintf( "FrameBuffer::Create() Error reading screen info for device and driver\r\n" );
        
        Destroy( );
		return -1;
	}
    
    // this returns as 10485760 (10 megs)
    // exactly enough for two 32bit buffers, allowing double buffering.
	mFrameBufferBytes = fixedScreenInfo.smem_len;
	
    // read the variable screen info so we can configure our preferred color and res
	result = ioctl( mFrameBufferHandle, FBIOGET_VSCREENINFO, &mVScreenInfo );
	if ( result == -1 )
    {
		flushPrintf( "FrameBuffer::Create() Error reading variable screen info\r\n" );
        
        Destroy( );
		return -1;
	}
    
    // set initial values
    mVScreenInfo.xoffset = 0;
    mVScreenInfo.yoffset = 0;
    
    // For bit depth, Looks like we can do 16 or 32 bit, but 
    // I'd imagine the emulator is 16 bit, so we'll stick to that.
	mVScreenInfo.xres           = PLATFORM_LCD_WIDTH;
	mVScreenInfo.yres           = PLATFORM_LCD_HEIGHT;
	mVScreenInfo.bits_per_pixel = PLATFORM_SCREEN_BPP;
    
    // for ref, 32bit is ARGB 8888
	
	// for 16 bit, the format is RGB 565
    mVScreenInfo.red.offset    = 11;
    mVScreenInfo.red.length    = 5;
    
    mVScreenInfo.green.offset  = 5;
    mVScreenInfo.green.length  = 6;
    
    mVScreenInfo.blue.offset   = 0;
    mVScreenInfo.blue.length   = 5;
    
    mVScreenInfo.transp.offset = 0;
    mVScreenInfo.transp.length = 0;
	
    // write our values so we update the display
	result = ioctl( mFrameBufferHandle, FBIOPUT_VSCREENINFO, &mVScreenInfo );
	if ( result == -1 )
    {
		flushPrintf( "FrameBuffer::Create() Error writing variable screen info\r\n" );
        
        Destroy( );
		return -1;
	}
	
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
    ClearFrameBuffer( );
    
    // ensure the display is showing the "top" half of the buffer
    mVScreenInfo.yoffset = 0;
    mVScreenInfo.xoffset = 0;
    result = ioctl( mFrameBufferHandle, FBIOPAN_DISPLAY, &mVScreenInfo );
    if ( result < 0 )
    {
        flushPrintf( "FrameBuffer::Create() Failed to pan display: %d errno: %d\r\n", result, errno );
        
        Destroy( );
        return -1;
    }
    
    // and setup our back buffer for the "bottom" half
    mpBackBuffer = mpFrameBuffer + FB_DOUBLE_BUFFER_OFFSET_BYTES;
	
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

void FrameBuffer::ClearFrameBuffer( )
{
    memset( mpFrameBuffer, 0x0, mFrameBufferBytes ); 
}

short *FrameBuffer::GetBackBuffer( )
{
    return (short *)mpBackBuffer;
}

void FrameBuffer::Flip( )
{
    // This treats the frame buffer as a double buffer.
    // The hardware screen is panned to one half of the buffer.
    // We render to the "other" half, and then pan the screen to it.
    // So if the buffer is
    // [ ]
    // [ ]
    // Then frame one goes:
    // [ ] <--This is being displayed
    // [X] <--we render here, then call ioctl PAN
    // Next frame:
    // [X] <--we now render here, then call ioctl PAN
    // [ ] <--This one is being displayed.
    
    // if we're _currently_ presenting the "top" half of the buffer,
    // make the "top" half the new back buffer and
    // pan to bottom
    if ( mVScreenInfo.yoffset == 0 )
    {
        mpBackBuffer = mpFrameBuffer;
        mVScreenInfo.yoffset = mVScreenInfo.yres;
    }
    // if we're _currently_ presenting the "bottom" half of the buffer,
    // make the "bottom" half the new back buffer and
    // pan to the top
    else
    {
        mpBackBuffer = mpFrameBuffer + FB_DOUBLE_BUFFER_OFFSET_BYTES;
        mVScreenInfo.yoffset = 0;
    }
    
    int result = ioctl( mFrameBufferHandle, FBIOPAN_DISPLAY, &mVScreenInfo );
    if ( result < 0 )
    {
        flushPrintf( "FrameBuffer::Flip() Pan error: %d errno: %d\r\n", result, errno );
    }
}

int FrameBufferCore::MVSXEnableHCD( )
{
    // I have no idea what this is doing - my best guess is
    // ensuring the owl driver is enabled
    FILE *pFile = fopen( "/proc/acts_hcd" , "w" );
	if( pFile == NULL )
    {
        return -1;
    }
	
    fputc( 'c', pFile );
    fclose( pFile );
    pFile = NULL;
    
    return 0;
}

void FrameBufferCore::PrintMode( fb_var_screeninfo *pVScreenInfo )
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

void FrameBufferCore::PrintVScreenInfo( fb_var_screeninfo *pVScreenInfo )
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

void FrameBufferCore::PrintFixedScreenInfo( fb_fix_screeninfo *pFixedScreenInfo )
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
