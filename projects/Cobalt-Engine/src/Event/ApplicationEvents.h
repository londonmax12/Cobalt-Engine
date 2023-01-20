#pragma once
#include "Event.h"

namespace Cobalt {
	class ApplicationCloseEvent : public Event {
	public:
		ApplicationCloseEvent() = default;

		EVENT_CLASS_TYPE(EventApplicationClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
}