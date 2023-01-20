#include "cobaltpch.h"

#include "MathUtils.h"
#include "MathContants.h"

float Cobalt::Math::Deg2Rad(float degrees)
{
	return (float)(degrees * DEG2RAD_MULT);
}

float Cobalt::Math::Rad2Deg(float radians)
{
	return (float)(radians * RAD2DEG_MULT);
}
