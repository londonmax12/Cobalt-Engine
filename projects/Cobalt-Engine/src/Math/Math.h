#pragma once
#include "Vector.h"
#include "Vertex.h"
#include "Matrix.h"

#include "MathContants.h"

namespace Cobalt {
	namespace Math {
		bool IsPowerOfTwo(int value);
		float Clamp(float value, float min, float max);
		int Clamp(int value, int min, int max);

		float Deg2Rad(float degrees);
		float Rad2Deg(float radians);
	}
}