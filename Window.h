#pragma once
#include <Windows.h>
#include "Renderer.h"
class Window
{
public:
	Window(int width, int height);
	void Render();
private:
	int width;
	int height;
	HINSTANCE hInstance;

	Renderer renderer;
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};