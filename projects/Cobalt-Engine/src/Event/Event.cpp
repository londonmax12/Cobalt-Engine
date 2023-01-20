#include "cobaltpch.h"
#include "Event.h"

bool Cobalt::EventSystem::Init()
{
    if (m_Instance) {
        COBALT_WARN("Event system already initialized");
        return false;
    }

    m_Instance = CreateRef<EventSystem>();
    
    return true;
}

void Cobalt::EventSystem::AddCallback(EventCallbackFn callback)
{
    if (!m_Instance)
    {
        COBALT_WARN("Failed to add event callback: Event system was not initialized");
        return;
    }
    m_Callbacks.push_back(callback);
}

void Cobalt::EventSystem::DispatchEvent(Event& ev)
{
    if (!m_Instance)
    {
        COBALT_WARN("Failed to dispatch events: Event system was not initialized");
        return;
    }
    for (auto& callback : m_Callbacks) {
        callback(ev);
    }
}
