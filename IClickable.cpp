#include "IClickable.h"
using namespace RosaEngine;
IClickable::IClickable( RectD _clickableArea)
	: isPressed(false), clickableArea(_clickableArea)
{

}
