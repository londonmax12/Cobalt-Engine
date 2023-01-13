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
		bool Init(RendererConfig config);
		void Shutdown();
	private:
		bool ResizeCallback(Ref<Event> ev);
	};
}