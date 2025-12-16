#pragma once

#include "Window.h"
#include "Renderer.h"
#include "UIManager.h"
namespace RosaEngine{
	class Application
	{
	public:
		int Run();
		void OnStart();
		void OnUpdate(float dt);
		void OnQuit();
		Application(int _sWidth, int _sHeight);
	private:
		std::unique_ptr<MyWindow> pWindow;
		std::unique_ptr<Renderer> pRenderer;
		std::unique_ptr<UIManager> pUIManager;
		std::unique_ptr<Mouse> pMouse;

		int screenWidth;
		int screenHeight;
	

	};
}