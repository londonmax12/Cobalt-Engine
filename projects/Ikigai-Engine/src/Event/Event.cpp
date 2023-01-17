#include "ikipch.h"
#include "Event.h"

bool Ikigai::EventSystem::Init()
{
    if (m_Instance) {
        IKIGAI_WARN("Event system already initialized");
        return false;
    }

    m_Instance = CreateRef<EventSystem>();
    
    return true;
}

void Ikigai::EventSystem::AddCallback(EventCallbackFn callback)
{
    if (!m_Instance)
    {
        IKIGAI_WARN("Failed to add event callback: Event system was not initialized");
        return;
    }
    m_Callbacks.push_back(callback);
}

void Ikigai::EventSystem::DispatchEvent(Event& ev)
{
    if (!m_Instance)
    {
        IKIGAI_WARN("Failed to dispatch events: Event system was not initialized");
        return;
    }
    for (auto& callback : m_Callbacks) {
        callback(ev);
    }
}
