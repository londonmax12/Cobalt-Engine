#include "cobaltpch.h"
#include "Renderer.h"

bool Cobalt::Renderer::Init(RendererConfig config)
{
	RendererBackendType type = config.Backend;

	m_Instance = CreateRef<Renderer>();
	m_Instance->m_Backend = CreateBackend(type);

	if (!m_Instance->m_Backend->Init(config.ApplicationName, config.State, config.Width, config.Height)) {
		COBALT_FATAL("Failed to init renderer backend");
		return false;
	}
	return true;
}

void Cobalt::Renderer::Shutdown()
{
	if (m_Instance->m_Backend) {
		m_Instance->m_Backend->Shutdown();
		delete m_Instance->m_Backend;
	}
}

void Cobalt::Renderer::OnEvent(Event& ev)
{
	EventDispatcher dispatcher(ev);
	dispatcher.Dispatch<SurfaceResizeEvent>(COBALT_BIND_EVENT_FN(OnResize));
}

bool Cobalt::Renderer::OnResize(SurfaceResizeEvent& ev)
{
	if (m_Backend)
		m_Backend->Resized(ev.GetWidth(), ev.GetHeight());

	return true;
}

bool Cobalt::Renderer::DrawFrame(RenderData* data)
{
	if (m_Backend->BeginFrame(data->DeltaTime)) {
		bool result = m_Backend->EndFrame(data->DeltaTime);

		if (!result) {
			COBALT_ERROR("Renderer failed to end frame");
			return false;
		}
	}
	return true;
}
