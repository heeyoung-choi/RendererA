#pragma once
#include <Windows.h>
#include "Helper.h"
#include <filesystem>
#include <shobjidl.h> // Common Item Dialog API
#include <string>
class MyWindow
{
public:
	MyWindow(int width, int height);
	void Render();
	std::optional<int> ProcessMessages();
	HWND& const GetHwnd (){  return hWnd; }
private:
	int width;
	int height;
	HINSTANCE hInstance;
	HWND hWnd;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	void InitUI(HWND hWnd);
};

std::wstring OpenFileExplorer(HWND hWndOwner);