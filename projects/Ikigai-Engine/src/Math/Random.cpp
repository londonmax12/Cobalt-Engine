#include "ikipch.h"

#include "Random.h"

namespace Ikigai {
	int Random::RandInt()
	{
		if (m_RandomSeeded) {
			srand((uint32_t)platform_GetAbsTime());
			m_RandomSeeded = true;
		}
		return rand();
	}
	int Random::RandInt(int min, int max)
	{
		if (m_RandomSeeded) {
			srand((uint32_t)platform_GetAbsTime());
			m_RandomSeeded = true;
		}
		return (rand() % (max - min + 1)) + min;
	}
	float Random::RandFloat()
	{
		return (float)RandInt() / (float)RAND_MAX;
	}
	float Random::RandFloat(int min, int max)
	{
		return min + ((float)RandInt() / ((float)RAND_MAX / (max - min)));
	}
}