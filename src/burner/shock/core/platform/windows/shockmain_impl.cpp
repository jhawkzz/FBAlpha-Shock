
// See License.md for license

#include "shock/includes.h"
#include "framebuffer_impl.h"

void ShockMainImpl::SetWindow(HWND hwnd)
{
    FrameBufferImpl::SetWindow(hwnd);
}

int ShockMainImpl::Update()
{
    MSG msg;

    if (PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
    {
        if (msg.message == WM_QUIT)
            return -1;

        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    return 0;
}
