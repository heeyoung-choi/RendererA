#pragma once
#include "Common.h"
namespace RosaEngine
{
	class Camera
	{
	public:
		Camera(XMFLOAT3 position, XMFLOAT3 target, XMFLOAT3 up)
			: position(position), target(target), up(up)
		{

		}
		XMFLOAT3 position, target, up;
	};
}
