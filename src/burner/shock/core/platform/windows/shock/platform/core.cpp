
// See License.md for license

#include "shock/includes.h"
#include "shock/core/framebuffer.h"

void CoreImpl::SetWindow(HWND hwnd)
{
    FrameBufferImpl::SetWindow(hwnd);
}

Core::~Core()
{}

int Core::Update()
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
