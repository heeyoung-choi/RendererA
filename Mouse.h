#pragma once
#include "UIManager.h"
namespace RosaEngine
{
	class Mouse
	{
	private:
		UIManager* uiManager;
	public:
		void HandleMouseMove(int mouseX, int mouseY);

		void HandleMouseDown(int mouseX, int mouseY);

		Mouse() : uiManager(nullptr) {}
		void SetUIManager(UIManager* manager)
		{
			uiManager = manager;
		}

	};
}