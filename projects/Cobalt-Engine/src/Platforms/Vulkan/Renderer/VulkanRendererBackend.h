#pragma once

#include "Renderer/RendererBackend.h"

#include <vulkan/vulkan.h>

#include "Platforms/Vulkan/VulkanStructs.h"
#include "Platforms/Vulkan/Debug/VulkanMessenger.h"

namespace Cobalt {
	class VulkanRendererBackend : public RendererBackend {
	public:
		bool Init(const char* applicationName, Ref<Platform::PlatformState> platformState, int width, int height);
		void Shutdown();
		void Resized(int width, int height);

		bool BeginFrame(DeltaTime dt);
		bool EndFrame(DeltaTime dt);

	private:
		Ref<VulkanState> m_State;

#ifdef COBALT_DEBUG_MODE
		Ref<VulkanMessenger> m_Debugger;
#endif
	};
}