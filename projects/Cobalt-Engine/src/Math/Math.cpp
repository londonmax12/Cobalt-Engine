#include "cobaltpch.h"

#include "Math.h"

namespace Cobalt {
	bool Math::IsPowerOfTwo(int value)
	{
		return (value != 0) && ((value & (value - 1)) == 0);
	}
}

