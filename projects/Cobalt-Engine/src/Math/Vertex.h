#pragma once
#include "Vector.h"

namespace Cobalt {
	struct Vertex3D
	{
		Vertex3D() = default;
		Vertex3D(float x, float y, float z);

		Vector3 Position;
	};
}