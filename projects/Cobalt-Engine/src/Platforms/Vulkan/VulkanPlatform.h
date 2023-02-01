#pragma once

#include "Platforms/Platform.h"
#include "Platforms/Vulkan/VulkanStructs.h"

#include <vulkan/vulkan.h>

#include <vector>

namespace Cobalt {
	namespace Platform {
		void GetVulkanExtensions(std::vector<const char*>* out);
		VkSurfaceKHR CreateVulkanSurface(PlatformState* platformState, Ref<VulkanState> vulkanState);
	}
}