#include "ikipch.h"
#include "Input.h"

bool Ikigai::Input::Init()
{
    EventSystem::Register(EventListener(EVENT_TYPE_MOUSEDOWN, MouseDownCallback));
    EventSystem::Register(EventListener(EVENT_TYPE_MOUSEUP, MouseUpCallback));
    return true;
}

bool Ikigai::Input::IsMouseButtonDown(int mouseButton)
{
    if (mouseButton > IkigaiMaxMouseButtons - 1)
    {
        IKIGAI_WARN("Ikigai only supports {} mouse buttons: {}", IkigaiMaxMouseButtons, mouseButton);
        return false;
    }
    return m_DownMouseButtons[mouseButton];
}

bool Ikigai::Input::MouseDownCallback(Ikigai::Ref<Ikigai::Event> ev)
{
    MouseDownEvent* e = (MouseDownEvent*)ev.get();
    if (e->MouseButton > IkigaiMaxMouseButtons - 1)
    {
        IKIGAI_WARN("Ikigai only supports {} mouse buttons: {}", IkigaiMaxMouseButtons, e->MouseButton);
        return false;
    }
    m_DownMouseButtons[e->MouseButton] = true;
    return false;
}

bool Ikigai::Input::MouseUpCallback(Ikigai::Ref<Ikigai::Event> ev)
{
    MouseDownEvent* e = (MouseDownEvent*)ev.get();
    if (e->MouseButton > IkigaiMaxMouseButtons - 1)
    {
        IKIGAI_WARN("Ikigai only supports {} mouse buttons: {}", IkigaiMaxMouseButtons, e->MouseButton);
        return false;
    }
    m_DownMouseButtons[e->MouseButton] = false;
    return false;
}
