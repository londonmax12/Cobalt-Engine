 #pragma once
#include "Event.h"
#include "Input/MouseCodes.h"

namespace Cobalt {
	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class MouseDownEvent : public MouseButtonEvent
	{
	public:
		MouseDownEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[MouseDownEvent](" << m_Button << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventMouseDown);
	};

	class MouseUpEvent : public MouseButtonEvent
	{
	public:
		MouseUpEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[MouseUpEvent](" << m_Button << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventMouseUp)
	};
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[MouseScrolledEvent](" << GetXOffset() << ", " << GetYOffset() << ")";
			return ss.str();
		}


		EVENT_CLASS_TYPE(EventMouseScroll)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	private:
		float m_XOffset, m_YOffset;
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(int x, float y)
			: m_X(y), m_Y(y) {}

		float GetX() const { return m_X; }
		float GetY() const { return m_Y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[MouseMovedEvent](" << GetX() << ", " << GetY() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventMouseMove)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	private:
		float m_X, m_Y;
	};
}