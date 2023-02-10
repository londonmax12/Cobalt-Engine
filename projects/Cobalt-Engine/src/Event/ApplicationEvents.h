#pragma once
#include "Event.h"

#include <sstream>

namespace Cobalt {
	class ApplicationCloseEvent : public Event {
	public:
		ApplicationCloseEvent() = default;

		EVENT_CLASS_TYPE(EventApplicationClose);
		EVENT_CLASS_CATEGORY(EventCategoryApplication);

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[ApplicationCloseEvent]";
			return ss.str();
		}
	};
}