#include "Mouse.h"
using namespace RosaEngine;

void Mouse::HandleMouseMove(int mouseX, int mouseY)
{
	if (uiManager)
	{
		uiManager->HandleMouseMove(mouseX, mouseY);
	}
}

void Mouse::HandleMouseDown(int mouseX, int mouseY)
{
	if (uiManager)
	{
		uiManager->HandleMouseDown(mouseX, mouseY);
	}
}
