#pragma once
#include "Event.h"

namespace Ikigai {
	class ApplicationCloseEvent : public Event {
	public:
		ApplicationCloseEvent() : Event(EVENT_TYPE_APPLICATION_CLOSE) {};
	};
}