#include "UIButton.h"

UIButton::UIButton(RectD _area, ColorD _baseColor,
	ColorD _textColor, float _pWidth,
	float _pHeight, WCHAR const* _buttonText, RectD _clickableArea)
	: IElement(_area),
	baseColor(_baseColor),
	textColor(_textColor),
	paddingWidth(_pWidth), paddingHeight(_pHeight),
	buttonText(_buttonText),
	isTextDirty(true),
	IClickable(_clickableArea)
{
}

HRESULT UIButton::CreateTextLayout(const ComPtr<IDWriteFactory>& dwriteFactory, const WCHAR const* const text, const ComPtr<IDWriteTextFormat>& textFormat)
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

void UIButton::Draw(ID2D1RenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteFactory* dWriteFactory, IDWriteTextFormat* textFormat)
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
	float alphaFactor = 1.0f;
	if (isHovered)
	{
		alphaFactor = 0.8f;
	}
	//set base color
	brush->SetColor(ColorD(
		baseColor.r,
		baseColor.g,
		baseColor.b,
		baseColor.a * alphaFactor
	));
	//draw the base rect for the button
	renderTarget->FillRectangle(&area, brush);
	//set text color
	brush->SetColor(ColorD(
		textColor.r,
		textColor.g,
		textColor.b,
		textColor.a * alphaFactor
	));
	renderTarget->DrawTextLayout(
		D2D1_POINT_2F{
			area.left + paddingWidth,
			area.top + paddingHeight
		},
		textLayout.Get(),
		brush,
		D2D1_DRAW_TEXT_OPTIONS_NO_SNAP
	);
}

void UIButton::OnMouseEnter()
{
	//SetText(L"Enter");
	isHovered = true;
}

void UIButton::OnMouseLeave()
{
	//SetText(L"Leave");

	isHovered = false;
}

void UIButton::OnMouseDown()
{
}

void UIButton::OnMouseUp()
{
}
	
bool UIButton::IsMouseIn(int mouseX, int mouseY)
{
	bool result =  (mouseX > area.left)
		&& (mouseX < area.right)
		&& (mouseY > area.top)
		&& (mouseY < area.bottom);
	if (result != isHovered)
	{
		if (result)
			OnMouseEnter();
		else
			OnMouseLeave();
	}
	return result;
}

float UIButton::GetWidth()
{
	return area.right - area.left;
}

float UIButton::GetHeight()
{
	return area.bottom - area.top;
}
