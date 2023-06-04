
// See License.md for license

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "shock/shock.h"
#include "shock/platform/core/framebuffer.h"

class FrameBuffer : public FrameBufferCore
{
public:
    static int  Create( int width, int height );
    static void Destroy( );
    static void ClearFrameBuffer( );
    static UINT16 *GetBackBuffer( );
    static void SetSize( int width, int height );
    static void GetSize( int *pWidth, int *pHeight );

    static void Flip( );
};

#endif
