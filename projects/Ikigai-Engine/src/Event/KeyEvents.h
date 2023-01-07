#pragma once

#include "Input/KeyCodes.h"
#include "Event/Event.h"

namespace Ikigai {
	class KeyDownEvent : public Event {
	public:
		KeyDownEvent(KeyCode keyCode) : Event(EVENT_TYPE_KEYDOWN), Key(keyCode) {};
		KeyCode Key;
	};
	class KeyUpEvent : public Event {
	public:
		KeyUpEvent(KeyCode keyCode) : Event(EVENT_TYPE_KEYUP), Key(keyCode) {};
		KeyCode Key;
	};
}