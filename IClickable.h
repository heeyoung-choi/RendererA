#pragma once
#include "Common.h"
namespace RosaEngine
{
	class IClickable
	{
	public:
		IClickable(RectD _clickableArea);
		virtual void OnLeftMouseDown() = 0;
		virtual void OnLeftMouseUp() = 0;
		bool IsPressed() const { return isPressed; };
		RectD GetClickableArea() const { return clickableArea; };
		void SetLeftMouseUpAction(Action action) { onLeftMouseUpAction = action; };
		void SetLeftMouseDownAction(Action action) { onLeftMouseDownAction = action; };
	protected:
		bool isPressed;
		RectD clickableArea;
		Action onLeftMouseUpAction;
		Action onLeftMouseDownAction;
	};
}