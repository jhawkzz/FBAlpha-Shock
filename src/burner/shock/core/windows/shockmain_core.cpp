
// See License.md for license

#include "../../includes.h"

void ShockMainCore::SetWindow(HWND hwnd)
{
    FrameBuffer::SetWindow(hwnd);
}

int ShockMainCore::Update()
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
