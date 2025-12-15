#include <Windows.h>
#include "Window.h"

// -----------------------------------------------------------------------------
// ENTRY POINT (WinMain)
// -----------------------------------------------------------------------------
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MyWindow window(1000, 1000);
	window.ProcessMessages();
}
