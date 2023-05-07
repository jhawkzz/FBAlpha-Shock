
// See License.md for license

#include "../../includes.h"

int ShockMainCore::Update()
{
    MSG msg;

    BOOL bRet = PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );

    if (bRet != -1)
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    return 0;
}
