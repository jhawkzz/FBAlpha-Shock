
// See License.md for license

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "framebuffer_impl.h"

class FrameBuffer : public FrameBufferImpl
{
public:
    static int  Create( );
    static void Destroy( );
    static void ClearFrameBuffer( );
    static short *GetBackBuffer( );

    static void Flip( );
};

#endif
