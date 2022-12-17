#include "ikipch.h"

#include "MathUtils.h"
#include "MathContants.h"

float Ikigai::Math::Deg2Rad(float degrees)
{
	return (float)(degrees * DEG2RAD_MULT);
}

float Ikigai::Math::Rad2Deg(float radians)
{
	return (float)(radians * RAD2DEG_MULT);
}
