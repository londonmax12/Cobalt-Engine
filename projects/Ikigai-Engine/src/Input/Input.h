#pragma once

#include "Event/MouseEvents.h"

#include <vector>

namespace Ikigai {
	class Input {
	public:
		static bool Init();

		static bool IsMouseButtonDown(int mouseButton);
	};
}