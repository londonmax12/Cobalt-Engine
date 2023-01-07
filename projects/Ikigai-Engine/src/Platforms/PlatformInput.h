#pragma once

#include "Input/Keycodes.h"
#include "Input/MouseCodes.h"

namespace Ikigai {
	namespace PlatformInput {
		KeyCode GetKeyCode(int platformKey);
		MouseCode GetMouseCode(int mouseButton);

		bool IsMouseButtonDown(int mouseButton);

		void Init();
	}
}