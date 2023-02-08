#include "cobaltpch.h"

#include "Math.h"

namespace Cobalt {
	bool Math::IsPowerOfTwo(int value)
	{
		return (value != 0) && ((value & (value - 1)) == 0);
	}
	float Math::Clamp(float value, float min, float max)
	{
		if (value > max)
			return max;
		if (value < min)
			return min;
		
		return value;
	}

	int Math::Clamp(int value, int min, int max)
	{
		if (value > max)
			return max;
		if (value < min)
			return min;

		return value;
	}

	float Math::Deg2Rad(float degrees)
	{
		return (float)(degrees * DEG2RAD_MULT);
	}

	float Math::Rad2Deg(float radians)
	{
		return (float)(radians * RAD2DEG_MULT);
	}

}

