#pragma once

#include "Renderer/RendererBackend.h"

#include <vulkan/vulkan.h>

namespace Ikigai {
	class VulkanRendererBackend : public RendererBackend {
	public:
		bool Init(const char* applicationName, Platform::PlatformState* platformState, int width, int height);
		void Shutdown();
		void Resized(int width, int height);

		bool BeginFrame(DeltaTime dt);
		bool EndFrame(DeltaTime dt);
	
	private:
		VkInstance m_Instance = nullptr;
		VkAllocationCallbacks* m_Allocator = nullptr;
	};
}