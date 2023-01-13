#pragma once
#include <vector>
#include <unordered_map>

#include "Application/Core.h"
#include <queue>

namespace Ikigai {
	enum EventType {
		EVENT_TYPE_KEYDOWN, EVENT_TYPE_KEYUP, 
		EVENT_TYPE_MOUSESCROLL, EVENT_TYPE_MOUSEUP, EVENT_TYPE_MOUSEDOWN,
		EVENT_TYPE_SURFACE_RESIZE,
		EVENT_TYPE_APPLICATION_CLOSE
	};

	class EventSystem;
	
	class Event {
	public:
		Event(EventType type) : m_Type(type) {}

		int GetPriority() const { return m_Priority; }
	protected:
		EventType m_Type;

		int m_Priority = 0;
		
		friend EventSystem;
	};

	struct EventCompare {
		bool operator()(Ref<Event> a, Ref<Event> b) {
			return a->GetPriority() > b->GetPriority();
		}
	};

	using EventCallback = bool(*)(Ref<Event>);

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

		static void PushEvent(Ref<Ikigai::Event> ev, int priotity = 0);

		static void PollEvents();

		static void Shutdown();
	private:
		static inline std::priority_queue<Ref<Event>, std::vector<Ref<Event>>, EventCompare> m_Events;
		static inline std::unordered_map<EventType, std::vector<EventListener>> m_Listeners;
	};
}