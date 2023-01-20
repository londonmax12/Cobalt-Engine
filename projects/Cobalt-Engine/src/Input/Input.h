#pragma once

#include "Event/MouseEvents.h"

#include <vector>

namespace Cobalt {
	class Input {
	public:
		static bool Init();

		static bool IsMouseButtonDown(int mouseButton);
	};
}