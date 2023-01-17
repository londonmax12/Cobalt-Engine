#include "ikipch.h"
#include "Renderer.h"

bool Ikigai::Renderer::Init(RendererConfig config)
{
    m_Instance = CreateRef<Renderer>();
    m_Instance->m_Backend = config.Backend;
    return true;
}

void Ikigai::Renderer::Shutdown()
{
}

void Ikigai::Renderer::OnEvent(Event& ev)
{
    EventDispatcher dispatcher(ev);
    dispatcher.Dispatch<SurfaceResizeEvent>(IKIGAI_BIND_EVENT_FN(Renderer::OnResize));
}

bool Ikigai::Renderer::OnResize(SurfaceResizeEvent& ev)
{
    IKIGAI_INFO("Resize from renderer: {}, {}", ev.GetWidth(), ev.GetHeight());
    return false;
}