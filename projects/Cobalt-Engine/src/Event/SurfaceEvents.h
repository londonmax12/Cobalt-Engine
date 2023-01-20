#pragma once
#include "Event.h"

namespace Cobalt {
	class SurfaceResizeEvent : public Event {
	public:
		SurfaceResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[SurfaceResizeEvent]" << "(" << m_Width << ", " << m_Height << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventSurfaceResize);
		EVENT_CLASS_CATEGORY(EventCategorySurface);
	private:
		unsigned int m_Width, m_Height;
	};
}