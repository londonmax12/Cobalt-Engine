#pragma once

#include "Application/Core.h"
#include "Event/SurfaceEvents.h"

namespace Ikigai {
	enum RendererBackend
	{
		RENDERER_BACKEND_NONE,
		RENDERER_BACKEND_VULKAN
	};

	struct RendererConfig
	{
		RendererConfig(RendererBackend backend) : Backend(backend) {}

		RendererBackend Backend = RENDERER_BACKEND_NONE;
	};

	class Renderer {
	public:
		static bool Init(RendererConfig config);
		static void Shutdown();

		void OnEvent(Event& ev);
		bool OnResize(SurfaceResizeEvent& ev);

		static Ref<Renderer> GetInstance() { return m_Instance; };
	private:
		RendererBackend m_Backend = RENDERER_BACKEND_NONE;

		inline static Ref<Renderer> m_Instance;
	};
}