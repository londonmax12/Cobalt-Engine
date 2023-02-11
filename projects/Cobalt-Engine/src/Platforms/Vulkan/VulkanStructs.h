#pragma once

#include <vulkan/vulkan.h>

#include "Application/Core.h"

namespace Cobalt {
	class VulkanDevice;
	class VulkanSwapchain;
	class VulkanRenderPass;
	class VulkanCommandBuffer;
	class VulkanFence;

	struct VulkanState {
		VkInstance Instance = nullptr;

		VkSurfaceKHR Surface = nullptr;
		VkAllocationCallbacks* Allocator = nullptr;

		Ref<VulkanDevice> Device = nullptr;
		Ref<VulkanSwapchain> Swapchain = nullptr;
		Ref<VulkanRenderPass> RenderPass = nullptr;

		int FramebufferWidth = 0;
		int FramebufferHeight = 0;

		int ImageIndex = 0;
		int CurrentFrame = 0;

		bool RecreatingSwapchain = false;

		std::vector<VulkanCommandBuffer> GraphicsCommandBuffers;

		std::vector<VulkanFence> InFlightFences;
		std::vector<Ref<VulkanFence>> ImagesInFlight;

		std::vector<VkSemaphore> ImageAvailableSemaphores;
		std::vector<VkSemaphore> QueueCompleteSemaphores;
	};

	struct VulkanSwapchainSupportInfo
	{
		VkSurfaceCapabilitiesKHR Capibilities{};
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};
}