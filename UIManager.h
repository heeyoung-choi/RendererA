
#pragma once
#include "UIButton.h"
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
	void CreateButton(RectD _area,
		ColorD _baseColor,
		ColorD _textColor,
		float _pWidth,
		float _pHeight, WCHAR const* _buttonText,
		RectD _clickableArea);
	void CreateButton(unique_ptr<UIButton> button)
	{
		elements.push_back(std::move(button));
	}
	
	HRESULT DrawUI(ID2D1RenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteFactory* dWriteFactory, IDWriteTextFormat* textFormat);
	void HandleMouseMove(int mouseX, int mouseY);
	void HandleMouseDown(int mouseX, int mouseY);
};