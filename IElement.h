#pragma once
#include "Common.h"
class IElement
{
public:
	IElement(RectD _area, bool _isHovered);
	RectD GetArea() const { return area; };
	virtual void OnMouseEnter() = 0;
	virtual void OnMouseLeave() = 0;
	virtual bool IsMouseIn(int mouseX, int mouseY) = 0;
	bool IsHovered() const { return isHovered; };
	virtual void Draw(const ComPtr<ID2D1RenderTarget>& renderTarget,
		const ComPtr<ID2D1SolidColorBrush>& brush, ComPtr<IDWriteFactory> dWriteFactory, ComPtr<IDWriteTextFormat> textFormat) = 0;
protected:
	bool isHovered;
	RectD area;
};