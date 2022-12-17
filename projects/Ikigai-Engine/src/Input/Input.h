#pragma once

#include "Event/MouseEvents.h"

constexpr int IkigaiMaxMouseButtons = 5;

namespace Ikigai {
	class Input {
	public:
		static bool Init();

		static bool IsMouseButtonDown(int mouseButton);
	private:
		static inline bool m_DownMouseButtons[IkigaiMaxMouseButtons] = { false, false, false, false, false };

		static bool MouseDownCallback(Ikigai::Ref<Ikigai::Event> ev);
		static bool MouseUpCallback(Ikigai::Ref<Ikigai::Event> ev);
	};
}