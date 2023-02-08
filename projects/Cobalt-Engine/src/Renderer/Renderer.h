#pragma once

#include "Application/Core.h"
#include "Event/SurfaceEvents.h"
#include "RendererBackend.h"

#include "Platforms/Vulkan/Renderer/VulkanRendererBackend.h"

namespace Cobalt {
	enum RendererBackendType
	{
		RENDERER_BACKEND_NONE,
		RENDERER_BACKEND_VULKAN
	};

	struct RendererConfig
	{
		RendererConfig(RendererBackendType backend) : Backend(backend) {}

		RendererBackendType Backend = RENDERER_BACKEND_NONE;
		Ref<Platform::PlatformState> State = nullptr;
		int Width = 1600;
		int Height = 900;
		const char* ApplicationName = "Cobalt Application";
	};

	struct RenderData {
		DeltaTime DeltaTime;
	};

	class Renderer {
	public:
		static bool Init(RendererConfig config);
		static void Shutdown();

		void OnEvent(Event& ev);
		bool OnResize(SurfaceResizeEvent& ev);

		bool DrawFrame(RenderData* data);

		static Ref<Renderer> GetInstance() { return m_Instance; };

		static RendererBackend* CreateBackend(RendererBackendType type) {
			switch (type)
			{
			case RENDERER_BACKEND_VULKAN: { return new VulkanRendererBackend(); }
			default: { return nullptr; }
			}
		}
	private:
		RendererBackend* m_Backend = nullptr;
		inline static Ref<Renderer> m_Instance;
	};
}