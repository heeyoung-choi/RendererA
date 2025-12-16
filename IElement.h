#pragma once
#include "Common.h"
#include <dwrite.h>
namespace RosaEngine
{
	enum class UIType { Button };

	class IElement
	{
	public:
		IElement(RectD _area);
		virtual ~IElement() = default;
		virtual UIType GetType() const = 0;
		RectD GetArea() const { return area; };
		virtual void OnMouseEnter() = 0;
		virtual void OnMouseLeave() = 0;
		virtual bool IsMouseIn(int mouseX, int mouseY) = 0;
		bool IsHovered() const { return isHovered; };
		virtual void Draw(ID2D1RenderTarget* renderTarget, ID2D1SolidColorBrush* brush, IDWriteFactory* dWriteFactory, IDWriteTextFormat* textFormat) = 0;

		bool IsActive() const { return isActive; };
		void SetActive(bool active) { isActive = active; };
	protected:
		bool isHovered;
		bool isActive;
		RectD area;
	};
}