#include "ikipch.h"

#include "Math.h"

namespace Ikigai {
	bool Math::IsPowerOfTwo(int value)
	{
		return (value != 0) && ((value & (value - 1)) == 0);
	}
}

