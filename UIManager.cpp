#include "UIManager.h"
using namespace RosaEngine;

void UIManager::CreateButton(RectD _area,
	ColorD _baseColor, ColorD _textColor, float _pWidth, float _pHeight, std::wstring _buttonText, RectD _clickableArea)
{
	elements.push_back(std::make_unique<UIButton>(
		_area,
		_baseColor,
		_textColor,
		_pWidth,
		_pHeight,
		_buttonText,
		_clickableArea
	));
}

HRESULT UIManager::DrawUI(ID2D1RenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteFactory* dWriteFactory, IDWriteTextFormat* textFormat)
{
	HRESULT hr = S_OK;
	if (renderTarget)
	{
		renderTarget->BeginDraw();
		for (const auto& element : elements)
		{
			if (!element->IsActive())
				continue;
			//button
			if (element->GetType() == UIType::Button)
			{
				element->Draw(renderTarget, brush, dWriteFactory, textFormat);
				continue;
			}
		}
		hr = renderTarget->EndDraw();
	}
	return hr;
	
}

void UIManager::HandleMouseMove(int mouseX, int mouseY)
{
	for (const auto& element : elements)
	{
		if (!element->IsActive())
			continue;
		if (element->GetType() == UIType::Button)
		element->IsMouseIn(mouseX, mouseY);
	}
}

void UIManager::HandleMouseDown(int mouseX, int mouseY)
{
	for (const auto& element : elements)
	{
		if (!element->IsActive())
			continue;
		if (element->GetType() == UIType::Button && element->IsMouseIn(mouseX, mouseY))
		{
			UIButton* button = static_cast<UIButton*>(element.get());
			if (button)
			{
				button->OnLeftMouseDown();
			}
		}
	}
}

