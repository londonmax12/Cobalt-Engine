#pragma once
#include "Event.h"

namespace Ikigai {
	class ApplicationCloseEvent : public Event {
	public:
		ApplicationCloseEvent() : Event(EVENTTYPE_APPLICATION_CLOSE) {};
	};
}