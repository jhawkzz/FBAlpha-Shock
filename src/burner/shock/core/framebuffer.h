
// See License.md for license

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "shock/shock.h"
#include "shock/platform/core/framebuffer.h"

class FrameBuffer : public FrameBufferCore
{
public:
    static int  Create( );
    static void Destroy( );
    static void ClearFrameBuffer( );
    static short *GetBackBuffer( );

    static void Flip( );
};

#endif
