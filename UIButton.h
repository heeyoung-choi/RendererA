#pragma once
#include "IElement.h"
#include "IClickable.h"


class UIButton : public IElement, public IClickable
{
public:
	UIButton(RectD _area, ColorD _baseColor,
		ColorD _textColor, float _pWidth,
		float _pHeight, std::wstring _buttonText, RectD _clickableArea);
	
	
	ColorD GetBaseColor() const { return baseColor; };
	HRESULT CreateTextLayout(const ComPtr<IDWriteFactory>& dwriteFactory,
		const WCHAR const* const text,
		const ComPtr<IDWriteTextFormat>& textFormat );
	void Draw(ID2D1RenderTarget* renderTarget,
		ID2D1SolidColorBrush* brush,
		IDWriteFactory* dWriteFactory,
		IDWriteTextFormat*  textFormat) override;
	void OnMouseEnter() override;
	void OnMouseLeave() override;
	void OnLeftMouseDown() override;
	void OnLeftMouseUp() override;
	bool IsMouseIn(int mouseX, int mouseY) override;
public:
	float GetWidth();
	float GetHeight();
	UIType GetType() const override { return UIType::Button; };
	std::wstring const GetText() const { return buttonText; };
	void SetText(std::wstring newText) {
		
		buttonText = newText; 
		isTextDirty = true;
	};
private:
	float paddingWidth, paddingHeight;
	std::wstring buttonText;
	ColorD baseColor, textColor;
	ComPtr<IDWriteTextLayout> textLayout;
	bool isTextDirty;

};