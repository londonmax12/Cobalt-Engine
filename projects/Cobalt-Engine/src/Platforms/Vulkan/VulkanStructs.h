#pragma once

#include <vulkan/vulkan.h>

#include "Application/Core.h"

namespace Cobalt {
	class VulkanDevice;

	struct VulkanState {
		VkInstance Instance = nullptr;

		VkSurfaceKHR Surface = nullptr;
		VkAllocationCallbacks* Allocator = nullptr;

		Ref<VulkanDevice> Device = nullptr;
	};
	struct VulkanSwapchainSupportInfo
	{
		VkSurfaceCapabilitiesKHR Capibilities;
		uint32_t FormatCount;
		VkSurfaceFormatKHR* Formats;
		uint32_t PresentModeCount;
		VkPresentModeKHR* PresentModes;
	};
}