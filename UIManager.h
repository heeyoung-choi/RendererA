
#pragma once
#include "UIButton.h"
#include "Renderer.h"
#include <vector>
#include <memory>
using std::vector;
using std::unique_ptr;
class UIManager
{
public:
	UIManager() {}
	UIManager(const UIManager&) = delete;
	UIManager& operator=(const UIManager&) = delete;
private:
	vector<unique_ptr<IElement>> elements;
public:
	void CreateButton(unique_ptr<Renderer> render,
		RectD _area,
		bool );
};