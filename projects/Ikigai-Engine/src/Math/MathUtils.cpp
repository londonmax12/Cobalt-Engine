#include "ikipch.h"

#include "MathUtils.h"
#include "MathContants.h"

float Ikigai::Math::Deg2Rad(float degrees)
{
	return degrees * DEG2RAD_MULT;
}

float Ikigai::Math::Rad2Deg(float radians)
{
	return radians * RAD2DEG_MULT;
}
