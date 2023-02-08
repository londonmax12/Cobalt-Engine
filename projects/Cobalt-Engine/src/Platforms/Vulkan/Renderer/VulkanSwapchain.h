#pragma once

#include "Platforms/Vulkan/VulkanStructs.h"
#include "Platforms/Vulkan/Renderer/VulkanImage.h"

#include <vulkan/vulkan.h>

namespace Cobalt {
	class VulkanSwapchain {
	private:
		VkSwapchainKHR m_Swapchain;

	public:
		bool Init(Ref<VulkanState> state);
		void Shutdown();

		void Create(int width, int height);
		void Recreate(int width, int height);
		
		bool GetNextImageIndex(unsigned int timeoutMs, VkSemaphore imageAvailableSemaphore, VkFence fence, int* outImageIndex);

		bool Present(VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, int presentImageIndex);

		operator VkSwapchainKHR() { return m_Swapchain; }

	private:
		void Destroy();

		unsigned int m_MaxFramesInFlight;

		VkSurfaceFormatKHR m_ImageFormat;
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;

		Ref<VulkanImage> m_DepthAttachment;
		Ref<VulkanState> m_State;
	};
}