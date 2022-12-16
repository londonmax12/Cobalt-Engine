#include "ikipch.h"
#include "Event.h"

void Ikigai::EventSystem::Register(EventListener listener)
{
	m_Listeners[listener.m_EventType].push_back(listener);
}

void Ikigai::EventSystem::Unregister(EventListener listener)
{
	EventCallback cb = listener.m_Callback;
	int index = -1;
	for (auto& listen : m_Listeners[listener.m_EventType]) {
		index++;
		if (listen.m_Callback == cb) {
			break;
		}
	}
	m_Listeners[listener.m_EventType].erase(m_Listeners[listener.m_EventType].begin() + index);
}

void Ikigai::EventSystem::PushEvent(Ikigai::Ref<Ikigai::Event> ev)
{
	m_Events.push_back(ev);
}

void Ikigai::EventSystem::PollEvents()
{
	for (Ref<Event> ev : m_Events) {
		for (auto& listener : m_Listeners[ev->m_Type]) {
			(*listener.m_Callback)(ev);
		}
		m_Events.erase(m_Events.begin());
	}
	m_Events.clear();
}

void Ikigai::EventSystem::Shutdown()
{
	m_Events.clear();
	m_Listeners.clear();
	IKIGAI_INFO("Shutdown event system");
}