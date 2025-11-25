#include <Windows.h>
#include "Window.h"

// -----------------------------------------------------------------------------
// ENTRY POINT (WinMain)
// -----------------------------------------------------------------------------
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window window(800, 600);
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        // Check for messages (Non-Blocking)
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // No messages? Render a frame!
			window.Render();
        }
    }

    

    return (int)msg.wParam;
}
