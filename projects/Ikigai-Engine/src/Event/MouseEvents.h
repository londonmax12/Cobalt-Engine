#pragma once
#include "Event.h"
#include "Input/MouseCodes.h"

namespace Ikigai {
	class MouseDownEvent : public Event {
	public:
		MouseDownEvent(MouseCode mouseButton) : Event(EVENT_TYPE_MOUSEDOWN), MouseButton(mouseButton) {};
		MouseCode MouseButton;
	};
	class MouseUpEvent : public Event {
	public:
		MouseUpEvent(MouseCode mouseButton) : Event(EVENT_TYPE_MOUSEUP), MouseButton(mouseButton) {};
		MouseCode MouseButton;
	};

	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(int delta) : Event(EVENT_TYPE_MOUSESCROLL), Delta(delta) {};
		int Delta;
	};
}