
// See License.md for license

#include "../../includes.h"

int FrameBuffer::Create( )
{    
	return -1;
}

void FrameBuffer::Destroy( )
{
}

void FrameBuffer::ClearFrameBuffer( )
{
}

short *FrameBuffer::GetBackBuffer( )
{
   return NULL;
}

void FrameBuffer::Flip( )
{
}


//LRESULT CALLBACK WndProc(
//    HWND hwnd,        // handle to window
//    UINT uMsg,        // message identifier
//    WPARAM wParam,    // first message parameter
//    LPARAM lParam)    // second message parameter
//{ 
//
//    switch (uMsg) 
//    { 
//    case WM_CREATE: 
//        // Initialize the window. 
//        return 0; 
//
//    case WM_PAINT: 
//        // Paint the window's client area. 
//        return 0; 
//
//    case WM_SIZE: 
//        // Set the size and position of the window. 
//        return 0; 
//
//    case WM_DESTROY: 
//        // Clean up window-specific data objects. 
//        return 0; 
//
//        // 
//        // Process other messages. 
//        // 
//
//    default: 
//        return DefWindowProc(hwnd, uMsg, wParam, lParam); 
//    } 
//    return 0; 
//}
//
//int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
//    LPSTR lpszCmdLine, int nCmdShow) 
//{ 
//    MSG msg;
//    BOOL bRet; 
//    WNDCLASS wc; 
//    UNREFERENCED_PARAMETER(lpszCmdLine); 
//
//    // Register the window class for the main window. 
//
//    if (!hPrevInstance) 
//    { 
//        wc.style = 0; 
//        wc.lpfnWndProc = (WNDPROC) WndProc; 
//        wc.cbClsExtra = 0; 
//        wc.cbWndExtra = 0; 
//        wc.hInstance = hInstance; 
//        wc.hIcon = LoadIcon((HINSTANCE) NULL, 
//            IDI_APPLICATION); 
//        wc.hCursor = LoadCursor((HINSTANCE) NULL, 
//            IDC_ARROW); 
//        wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
//        wc.lpszMenuName =  "MainMenu"; 
//        wc.lpszClassName = "MainWndClass"; 
//
//        if (!RegisterClass(&wc)) 
//            return FALSE; 
//    } 
//
//    hAppInst = hInstance;  // save instance handle 
//
//    // Create the main window. 
//
//    hwndMain = CreateWindow("MainWndClass", "Sample", 
//        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
//        CW_USEDEFAULT, CW_USEDEFAULT, (HWND) NULL, 
//        (HMENU) NULL, hAppInst, (LPVOID) NULL); 
//
//    // If the main window cannot be created, terminate 
//    // the application. 
//
//    if (!hwndMain) 
//        return FALSE; 
//
//    // Show the window and paint its contents. 
//
//    ShowWindow(hwndMain, nCmdShow); 
//    UpdateWindow(hwndMain); 
//
//    // Start the message loop. 
//
//    while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
//    { 
//        if (bRet == -1)
//        {
//            // handle the error and possibly exit
//        }
//        else
//        {
//            TranslateMessage(&msg); 
//            DispatchMessage(&msg); 
//        }
//    } 
//
//    // Return the exit code to the system. 
//
//    return msg.wParam; 
//} 