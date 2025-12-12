#include "Window.h"

#include <exception>
Window::Window(int width, int height)
{

    //get a module handle
	hInstance = GetModuleHandle(NULL);
    // 1. REGISTER WINDOW CLASS
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMsgSetup;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "DirectXGameWindow"; // 'L' for Unicode string

    RegisterClassEx(&wc);

    // 2. CREATE THE WINDOW
    // We adjust the window size so the internal "client area" is exactly 800x600
    RECT wr = { 0, 0, width, height };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowEx(
        0,
        "DirectXGameWindow",
        "My First DirectX 11 Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        wr.right - wr.left, wr.bottom - wr.top, // Adjusted width/height
        NULL,
        NULL,
        hInstance,
        this);

    if (!hWnd) throw std::exception ("failed to create window");

    ShowWindow(hWnd, SW_SHOWDEFAULT);
	renderer.InitD3D(hWnd, height, width);
	InitUI();
}

void Window::Render()
{
	renderer.RenderFrame();

	uiManager.DrawUI(renderer.g_pBackBufferRT.Get(), renderer.g_pBrushUIMain.Get(), renderer.g_pDWriteFactory.Get(), renderer.g_pTextFormat.Get());


	renderer.Present();
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window instance handler
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // retrieve ptr to window instance
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window instance handler
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
			return 0;
        case WM_MOUSEMOVE:
            {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            // Handle mouse movement, e.g., update UI elements
			uiManager.HandleMouseMove(mouseX, mouseY);
            break;
		}
        case WM_LBUTTONDOWN:
            {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
			// Handle mouse button down, e.g., check if a UI element was clicked
            uiManager.HandleMouseDown(mouseX, mouseY);
			break;
		    }
        default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return S_OK;
}

void Window::InitUI()
{
    wchar_t const* m1 = L"Hello World";
    auto mybutton = std::make_unique<UIButton>(
        RectD{ 300.0f, 50.0f, 500.0f, 150.0f },
        ColorD{ 0.8f, 0.2f, 0.2f, 1.0f },
        ColorD{ 1.0f, 1.0f, 1.0f, 1.0f },
        10.0f,
        10.0f,
        m1,
		RectD{ 300.0f, 50.0f, 500.0f, 150.0f });
    
	UIButton* buttonPtr = mybutton.get();
    mybutton->SetLeftMouseDownAction([buttonPtr]() {
		buttonPtr->SetText(L"Clicked!");
		});
	uiManager.CreateButton(std::move(mybutton));
    // Use buffer...
   // free(buffer); // Remember to free when done
}
