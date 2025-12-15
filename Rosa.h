#pragma once
#include "Window.h"
#include "Renderer.h"
#include "UIManager.h"
class RosaEngine
{
public:
	int Run();
	void OnStart();
	void OnUpdate(float dt);
	void OnQuit();
	RosaEngine(int _sWidth, int _sHeight);
private:
	std::unique_ptr<MyWindow> pWindow;
	std::unique_ptr<Renderer> pRenderer;
	std::unique_ptr<UIManager> pUIManager;

	int screenWidth;
	int screenHeight;
	

};