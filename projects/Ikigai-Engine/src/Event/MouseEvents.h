#pragma once
#include "Event.h"

namespace Ikigai {
	class MouseDownEvent : public Event {
	public:
		MouseDownEvent(int mouseButton) : Event(EVENTTYPE_MOUSEDOWN), MouseButton(mouseButton) {};
		int MouseButton;
	};
	class MouseUpEvent : public Event {
	public:
		MouseUpEvent(int mouseButton) : Event(EVENTTYPE_MOUSEUP), MouseButton(mouseButton) {};
		int MouseButton;
	};

	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(int delta) : Event(EVENTTYPE_MOUSESCROLL), Delta(delta) {};
		int Delta;
	};
}