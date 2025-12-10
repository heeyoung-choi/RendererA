#include "UIButton.h"

UIButton::UIButton(RectD _area, ColorD _baseColor,
	ColorD _textColor, float _pWidth,
	float _pHeight, WCHAR* _buttonText RectD _clickableArea)
	: IElement(_area, _isHovered),
	baseColor(_baseColor),
	textColor(_textColor),
	paddingWidth(_pWidth), paddingHeight(_pHeight),
	buttonText(_buttonText),
	isTextDirty(true),
	IClickable(_isPressed, _clickableArea)
{
}

HRESULT UIButton::CreateTextLayout(const ComPtr<IDWriteFactory>& dwriteFactory, const WCHAR* const text, const ComPtr<IDWriteTextFormat>& textFormat)
{
	return dwriteFactory.Get()->CreateTextLayout(
		text,
		lstrlenW(text),
		textFormat.Get(),
		GetWidth() - 2 * paddingWidth,
		GetHeight() - 2 * paddingHeight,
		textLayout.GetAddressOf()

	);
}

void UIButton::Draw(const ComPtr<ID2D1RenderTarget>& renderTarget, const ComPtr<ID2D1SolidColorBrush>& brush, ComPtr<IDWriteFactory> dWriteFactory, ComPtr<IDWriteTextFormat> textFormat)
{
	if (isTextDirty)
	{
		//recreate text layout
		//Assuming textFormat and dwriteFactory are accessible here
		//You might need to pass them as parameters or store them as members
		//CreateTextLayout(dwriteFactory, buttonText, textFormat);
		isTextDirty = false;
		CreateTextLayout(dWriteFactory,
			buttonText,
			textFormat
			);
	}
	//set base color
	brush.Get()->SetColor(ColorD(
		baseColor.r,
		baseColor.g,
		baseColor.b,
		baseColor.a// * (isPressed ? 0.7f : 1.0f)
	));
	//draw the base rect for the button
	renderTarget.Get()->FillRectangle(&area, brush.Get());
	//set text color
	brush.Get()->SetColor(ColorD(
		textColor.r,
		textColor.g,
		textColor.b,
		textColor.a //* (isPressed ? 0.7f : 1.0f)
	));
	renderTarget.Get()->DrawTextLayout(
		D2D1_POINT_2F{
			area.left + paddingWidth,
			area.top + paddingHeight
		},
		textLayout.Get(),
		brush.Get(),
		D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
	);
}

void UIButton::OnMouseEnter()
{
	
}

void UIButton::OnMouseLeave()
{

}
	
bool UIButton::IsMouseIn(int mouseX, int mouseY)
{
	return (mouseX > area.left)
		&& (mouseX < area.right)
		&& (mouseY > area.top)
		&& (mouseY < area.bottom);
}

float UIButton::GetWidth()
{
	return area.right - area.left;
}

float UIButton::GetHeight()
{
	return area.bottom - area.top;
}
