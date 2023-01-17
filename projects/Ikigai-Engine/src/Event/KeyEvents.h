#pragma once

#include "Input/KeyCodes.h"
#include "Event/Event.h"

namespace Ikigai {
	class KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class KeydownEvent : public KeyEvent
	{
	public:
		KeydownEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[KeydownEvent](" << m_KeyCode << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventKeydown)
	};

	class KeyupEvent : public KeyEvent
	{
	public:
		KeyupEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[KeyupEvent](" << m_KeyCode << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(EventKeyup)
	};

}