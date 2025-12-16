#pragma once
#include <Windows.h>
#include "Helper.h"
#include "Mouse.h"
#include <filesystem>
#include <shobjidl.h> // Common Item Dialog API
#include <string>
namespace RosaEngine {
	class MyWindow
	{
	public:
		MyWindow(int width, int height);
		void Render();
		std::optional<int> ProcessMessages();
		HWND& const GetHwnd() { return hWnd; }
		void SetMouse(Mouse* mouse)
		{
			pMouse = mouse;
		}
	private:
		int width;
		int height;
		HINSTANCE hInstance;
		HWND hWnd;

		Mouse* pMouse;
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		void InitUI(HWND hWnd);
	};
}
std::wstring OpenFileExplorer(HWND hWndOwner);