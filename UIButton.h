#pragma once
#include "IElement.h"
#include "IClickable.h"
#include <dwrite.h>

class UIButton : public IElement, public IClickable
{
public:
	UIButton(RectD _area, ColorD _baseColor,
		ColorD _textColor, float _pWidth,
		float _pHeight, WCHAR * _buttonText RectD _clickableArea);
	
	
	ColorD GetBaseColor() const { return baseColor; };
	HRESULT CreateTextLayout(const ComPtr<IDWriteFactory>& dwriteFactory,
		const WCHAR* const text,
		const ComPtr<IDWriteTextFormat>& textFormat );
	void Draw(const ComPtr<ID2D1RenderTarget>& renderTarget,
		const ComPtr<ID2D1SolidColorBrush>& brush, ComPtr<IDWriteFactory> dWriteFactory, ComPtr<IDWriteTextFormat> textFormat) override;
	void OnMouseEnter() override;
	void OnMouseLeave() override;
	bool IsMouseIn(int mouseX, int mouseY) override;
public:
	float GetWidth();
	float GetHeight();
	WCHAR* GetText() const { return buttonText; };
	void SetText(WCHAR* newText) { 
		buttonText = newText; 
		isTextDirty = true;
	};
private:
	float paddingWidth, paddingHeight;
	WCHAR* buttonText;
	ColorD baseColor, textColor;
	ComPtr<IDWriteTextLayout> textLayout;
	bool isTextDirty;

};