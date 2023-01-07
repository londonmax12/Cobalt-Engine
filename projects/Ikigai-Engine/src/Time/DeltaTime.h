#pragma once
#include "Timer.h"

namespace Ikigai {
	class DeltaTime : public Timer
	{
	public:
		operator float()
		{
			return Elapsed();
		}
	private:

	};
}
