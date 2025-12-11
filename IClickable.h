#pragma once
#include "Common.h"
class IClickable
{
public:
	IClickable(RectD _clickableArea);
	virtual void OnMouseDown() = 0;
	virtual void OnMouseUp() = 0;
	bool IsPressed() const { return isPressed; };
	RectD GetClickableArea() const { return clickableArea; };
protected:
	bool isPressed;
	RectD clickableArea;
};