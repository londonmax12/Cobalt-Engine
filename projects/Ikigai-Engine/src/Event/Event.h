#pragma once
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>

#include "Logging/Logging.h"
#include "Application/Core.h"

namespace Ikigai {
	enum EventType {
		EventKeydown, EventKeyup,
		EventMouseScroll, EventMouseUp, EventMouseDown, EventMouseMove,
		EventSurfaceResize,
		EventApplicationClose,

		EventNone
    };

	enum EventCategory
	{
		EventCategoryNone = 0,
		EventCategoryApplication = IKIGAI_BIT(0),
		EventCategorySurface = IKIGAI_BIT(1),
		EventCategoryInput = IKIGAI_BIT(2),
		EventCategoryKeyboard = IKIGAI_BIT(3),
		EventCategoryMouse = IKIGAI_BIT(4),
		EventCategoryMouseButton = IKIGAI_BIT(5)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	class EventSystem {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		static bool Init();

		static Ref<EventSystem> GetInstance() { return m_Instance; }

		void AddCallback(EventCallbackFn callback);
		void DispatchEvent(Event& ev);

	private:
		inline static Ref<EventSystem> m_Instance;
		
		std::vector<EventCallbackFn> m_Callbacks;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}