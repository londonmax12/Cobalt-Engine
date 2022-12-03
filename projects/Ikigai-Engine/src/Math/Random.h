#pragma once
#include "Platforms/Platform.h"


namespace Ikigai {
	class Random {

	public:
		static int RandInt();
		static int RandInt(int min, int max);

		static float RandFloat();
		static float RandFloat(int min, int max);
	private:
		inline static bool m_RandomSeeded = false;
	};
}