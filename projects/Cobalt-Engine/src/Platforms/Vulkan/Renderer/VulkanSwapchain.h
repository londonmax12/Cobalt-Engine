#pragma once

#include "Platforms/Vulkan/VulkanStructs.h"
#include "Platforms/Vulkan/Renderer/VulkanImage.h"

#include <vulkan/vulkan.h>

namespace Cobalt {
	class VulkanSwapchain {
	private:
		VkSwapchainKHR m_Swapchain;
		VkSurfaceFormatKHR m_ImageFormat;

		std::vector<VkImage> m_Images;
	public:
		bool Init(Ref<VulkanState> state);
		void Shutdown();

		void Create(int width, int height);
		void Recreate(int width, int height);
		
		bool GetNextImageIndex(unsigned int timeoutMs, VkSemaphore imageAvailableSemaphore, VkFence fence, int* outImageIndex);

		int GetImageCount() { return m_Images.size(); }

		bool Present(VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, int presentImageIndex);

		operator VkSwapchainKHR() { return m_Swapchain; }

		VkSurfaceFormatKHR GetFormat() { return m_ImageFormat; }

	private:
		void Destroy();

		unsigned int m_MaxFramesInFlight;

		std::vector<VkImageView> m_ImageViews;

		Ref<VulkanImage> m_DepthAttachment;
		Ref<VulkanState> m_State;
	};
}