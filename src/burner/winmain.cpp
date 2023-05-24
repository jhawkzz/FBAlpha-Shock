
#include "shock/core/core.h"
#include "shock/core/framebuffer.h"

HINSTANCE hAppInst;
HWND hwndMain;

CDEmuDo isowavDo;

void (*cBurnerKeyCallback)(UINT8 code, UINT8 KeyType, UINT8 down);
INT32 nReplayExternalDataCount;
UINT8 *ReplayExternalData;

INT32 VidSNewTinyMsg(const wchar_t* pText, INT32 nRGB = 0, INT32 nDuration = 0, INT32 nPriority = 5)
{
    return 0;
}

LRESULT CALLBACK WndProc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{ 

    switch (uMsg) 
    { 
    case WM_CREATE: 
        // Initialize the window. 
        return 0; 

    case WM_PAINT: 
        FrameBuffer::Blit();
        return 0; 

    case WM_SIZE: 
        // Set the size and position of the window. 
        return 0; 

    case WM_DESTROY: 
        PostQuitMessage(0);
        return 0; 

        // 
        // Process other messages. 
        // 

    default: 
        return DefWindowProc(hwnd, uMsg, wParam, lParam); 
    } 
    return 0; 
}

#include <shellapi.h>

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpszCmdLine, int nCmdShow) 
{ 
    WNDCLASS wc = {}; 
    UNREFERENCED_PARAMETER(lpszCmdLine); 

    // Register the window class for the main window. 

    if (!hPrevInstance) 
    { 
        wc.style = 0; 
        wc.lpfnWndProc = (WNDPROC) WndProc; 
        wc.cbClsExtra = 0; 
        wc.cbWndExtra = 0; 
        wc.hInstance = hInstance; 
        wc.hIcon = LoadIcon((HINSTANCE) NULL, 
            IDI_APPLICATION); 
        wc.hCursor = LoadCursor((HINSTANCE) NULL, 
            IDC_ARROW); 
        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
        wc.lpszClassName = "ShockCls"; 

        if (!RegisterClass(&wc)) 
            return 0; 
    } 

    hAppInst = hInstance;  // save instance handle 

    RECT rect = { 0, 0, PLATFORM_LCD_WIDTH, PLATFORM_LCD_HEIGHT };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hwndMain = CreateWindow("ShockCls", "Shock", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
        rect.right - rect.left, rect.bottom - rect.top, (HWND) NULL,
        (HMENU) NULL, hAppInst, (LPVOID) NULL); 

    if (!hwndMain) 
        return 0; 

    ShowWindow(hwndMain, nCmdShow); 
    UpdateWindow(hwndMain); 

    CoreCore::SetWindow(hwndMain);

    int count;

    LPWSTR *argList = CommandLineToArgvW(GetCommandLineW(), &count);
    if (!argList || count <= 1)
        return 0;
    
    char str[256];
    WideCharToMultiByte(CP_ACP, 0, argList[1], -1, str, sizeof(str), NULL, NULL);

    LocalFree(argList);

    return ShockMain::Run(str);
} 