#pragma once
#include <vector>
#include <unordered_map>

#include "Application/Core.h"

namespace Ikigai {
	enum EventType {
		EVENT_TYPE_KEYPRESS, EVENT_TYPE_KEYDOWN, EVENT_TYPE_KEYUP, 
		EVENT_TYPE_MOUSESCROLL, EVENT_TYPE_MOUSEUP, EVENT_TYPE_MOUSEDOWN,
		EVENT_TYPE_SURFACE_RESIZE,
		EVENT_TYPE_APPLICATION_CLOSE
	};

	class EventSystem;
	
	class Event {
	public:
		Event(EventType type) : m_Type(type) {}
	protected:
		EventType m_Type;

		friend EventSystem;
	};

	typedef bool (*EventCallback)(Ref<Event> e);


	class EventListener {
	public:
		EventListener(EventType type, EventCallback callback) : m_EventType(type), m_Callback(callback) {};
	private:
		EventCallback m_Callback;
		EventType m_EventType;

		friend EventSystem;
	};
	 
	class EventSystem {
	public:
		static void Register(EventListener listener);
		static void Unregister(EventListener listener);

		static void PushEvent(Ref<Ikigai::Event> ev);

		static void PollEvents();

		static void Shutdown();
	private:
		static inline std::vector<Ref<Ikigai::Event>> m_Events;
		static inline std::unordered_map<EventType, std::vector<EventListener>> m_Listeners;
	};
}