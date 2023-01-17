#include "ikipch.h"
#include "Renderer.h"

bool Ikigai::Renderer::Init(RendererConfig config)
{
	RendererBackendType type = config.Backend;

	m_Instance = CreateRef<Renderer>();
	m_Instance->m_Backend = CreateBackend(type);

	if (!m_Instance->m_Backend->Init(config.ApplicationName, config.State, config.Width, config.Height)) {
		IKIGAI_FATAL("Failed to init renderer backend");
		return false;
	}
	return true;
}

void Ikigai::Renderer::Shutdown()
{
	m_Instance->m_Backend->Shutdown();
	delete m_Instance->m_Backend;
}

void Ikigai::Renderer::OnEvent(Event& ev)
{
	EventDispatcher dispatcher(ev);
	dispatcher.Dispatch<SurfaceResizeEvent>(IKIGAI_BIND_EVENT_FN(OnResize));
}

bool Ikigai::Renderer::OnResize(SurfaceResizeEvent& ev)
{
	if (m_Backend)
		m_Backend->Resized(ev.GetWidth(), ev.GetHeight());

	return true;
}

bool Ikigai::Renderer::DrawFrame(RenderData* data)
{
	if (m_Backend->BeginFrame(data->DeltaTime)) {
		bool result = m_Backend->EndFrame(data->DeltaTime);

		if (!result) {
			IKIGAI_ERROR("Renderer failed to end frame");
			return false;
		}
	}
	return true;
}
