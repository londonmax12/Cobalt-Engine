#pragma once

#include "Platforms/Vulkan/VulkanStructs.h"
#include "Platforms/Vulkan/Renderer/VulkanImage.h"
#include "Platforms/Vulkan/Renderer/VulkanFramebuffer.h"

#include <vulkan/vulkan.h>

namespace Cobalt {
	class VulkanSwapchain {
	private:
		VkSwapchainKHR m_Swapchain;

		VkSurfaceFormatKHR m_ImageFormat;
		std::vector<VkImage> m_Images;
		std::vector<VkImageView> m_ImageViews;
		Ref<VulkanImage> m_DepthAttachment;
		std::vector<Ref<VulkanFramebuffer>> m_Framebuffers;

		unsigned int m_MaxFramesInFlight;
	public:
		bool Init(Ref<VulkanState> state);
		void Shutdown();

		void Create(int width, int height);
		void Recreate(int width, int height);

		bool Present(VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, int presentImageIndex);

		operator VkSwapchainKHR() { return m_Swapchain; }

		bool GetNextImageIndex(unsigned int timeoutMs, VkSemaphore imageAvailableSemaphore, VkFence fence, int* outImageIndex);
		int GetImageCount() { return (int)m_Images.size(); }
		unsigned int GetMaxFramesInFlight() { return m_MaxFramesInFlight; }
		VkSurfaceFormatKHR GetFormat() { return m_ImageFormat; }
		Ref<VulkanImage> GetDepthAttachment() { return m_DepthAttachment; }

		void RegenerateFramebuffers(Ref<VulkanRenderPass> renderPass);
	private:
		void Destroy();

		Ref<VulkanState> m_State;
	};
}