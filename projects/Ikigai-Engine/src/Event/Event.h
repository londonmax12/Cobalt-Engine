#include <vector>
#include <unordered_map>

namespace Ikigai {
	enum EventType {
		EVENTTYPE_KEYPRESS, EVENTTYPE_KEYDOWN, EVENTTYPE_KEYUP, EVENTTYPE_MOUSECLICK, EVENTTYPE_MOUSEDOWN, EVENTTYPE_MOUSEUP,
		EVENTTYPE_WINDOW_RESIZE, EVENTTYPE_WINDOW_CLOSE
	};

	class EventSystem;
	
	class Event {
	public:
		Event(EventType type) : m_Type(type) {}
	protected:
		EventType m_Type;

		friend EventSystem;
	};

	typedef bool (*EventCallback)(Event& e);


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

		static void PushEvent(Event ev);

		static void PollEvents();

		static void Shutdown();
	private:
		static inline std::vector<Event> m_Events;
		static inline std::unordered_map<EventType, std::vector<EventListener>> m_Listeners;
	};
}