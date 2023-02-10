#include "cobaltpch.h"

#include "VulkanSwapchain.h"

#include "Platforms/Vulkan/Renderer/VulkanDevice.h"
#include "Platforms/Vulkan/Utility/VulkanUtility.h"
#include "Logging/Logging.h"
#include "Math/Math.h"

bool Cobalt::VulkanSwapchain::Init(Ref<VulkanState> state)
{
	m_State = state;
	Create(state->FramebufferWidth, state->FramebufferHeight);
	return true;
}

void Cobalt::VulkanSwapchain::Shutdown()
{
	Destroy();
}

void Cobalt::VulkanSwapchain::Create(int width, int height)
{
	VkExtent2D swapchainExtent = { (uint32_t)width, (uint32_t)height };
	m_MaxFramesInFlight = 2;

	Ref<VulkanSwapchainSupportInfo> supportInfo = m_State->Device->GetSwapchainSupportInfo();
	bool found = false;
	for (auto& format : supportInfo->Formats) {
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			m_ImageFormat = format;
			found = true;
			break;
		}
	}

	if (!found) {
		if (!supportInfo->Formats.size()) {
			COBALT_FATAL("Device has no image formats");
			return;
		}
		else {
			COBALT_WARN("Device does not support prefered image format");
			m_ImageFormat = supportInfo->Formats[0];
		}
	}

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (auto& mode : supportInfo->PresentModes) {
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = mode;
			break;
		}
		if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			presentMode = mode;
		}
	}

	m_State->Device->RequerySwapchainSupport();
	supportInfo = m_State->Device->GetSwapchainSupportInfo();

	if (supportInfo->Capibilities.currentExtent.width == uint32_t(-1))
	{
		COBALT_ERROR("Current extent was invalid");
		return;
	}

	unsigned int imageCount = supportInfo->Capibilities.minImageCount + 1;
	if (supportInfo->Capibilities.maxImageCount > 0 && imageCount > supportInfo->Capibilities.maxImageCount) {
		imageCount = supportInfo->Capibilities.maxImageCount;
	}
	
	swapchainExtent = supportInfo->Capibilities.currentExtent;

	VkExtent2D minExtent = supportInfo->Capibilities.minImageExtent;
	VkExtent2D maxExtent = supportInfo->Capibilities.maxImageExtent;
	swapchainExtent.width = Math::Clamp((int)swapchainExtent.width, (int)minExtent.width, (int)maxExtent.width);
	swapchainExtent.height = Math::Clamp((int)swapchainExtent.height, (int)minExtent.height, (int)maxExtent.height);

	VkSwapchainCreateInfoKHR swapchainCreateInfo{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
	swapchainCreateInfo.surface = m_State->Surface;
	swapchainCreateInfo.minImageCount = imageCount;
	swapchainCreateInfo.imageFormat = m_ImageFormat.format;
	swapchainCreateInfo.imageColorSpace = m_ImageFormat.colorSpace;
	swapchainCreateInfo.imageExtent = swapchainExtent;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (m_State->Device->GetGraphicsQueueIndex() != m_State->Device->GetPresentQueueIndex()) {
		uint32_t queueFamilyIndices[2] = {(uint32_t)m_State->Device->GetGraphicsQueueIndex(), (uint32_t)m_State->Device->GetPresentQueueIndex()};
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = 2;
		swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 0;
		swapchainCreateInfo.pQueueFamilyIndices = 0;
	}

	swapchainCreateInfo.preTransform = supportInfo->Capibilities.currentTransform;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.presentMode = presentMode;
	swapchainCreateInfo.clipped = true;
	swapchainCreateInfo.oldSwapchain = 0;

	{
		VkResult result = vkCreateSwapchainKHR(*m_State->Device, &swapchainCreateInfo, m_State->Allocator, &m_Swapchain);
		if (!VK_CHECK(result)) return;
	}

	uint32_t imageCount2 = 0;
	{
		VkResult result = vkGetSwapchainImagesKHR(*m_State->Device, m_Swapchain, &imageCount2, 0);
		if (!VK_CHECK(result)) return;
	}
	if (!m_Images.size()) {
		m_Images = std::vector<VkImage>(imageCount2);
	}	
	if (!m_ImageViews.size()) {
		m_ImageViews = std::vector<VkImageView>(imageCount2);
	}
	vkGetSwapchainImagesKHR(*m_State->Device, m_Swapchain, &imageCount2, m_Images.data());

	for (int i = 0; i < m_Images.size(); i++) {
		VkImageViewCreateInfo imageCreateInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		imageCreateInfo.image = m_Images[i];
		imageCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageCreateInfo.format = m_ImageFormat.format;
		imageCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCreateInfo.subresourceRange.baseMipLevel = 0;
		imageCreateInfo.subresourceRange.levelCount = 1;
		imageCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageCreateInfo.subresourceRange.layerCount = 1;
	
		VK_CHECK(vkCreateImageView(*m_State->Device, &imageCreateInfo, m_State->Allocator, &m_ImageViews[i]));
	}

	m_DepthAttachment = CreateRef<VulkanImage>();

	VulkanImageConfig conf{};
	conf.ImageType = VK_IMAGE_TYPE_2D;
	conf.Width = swapchainExtent.width;
	conf.Height = swapchainExtent.height;
	conf.Format = m_State->Device->GetDepthFormat();
	conf.Tiling = VK_IMAGE_TILING_OPTIMAL;
	conf.UsageFlags = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	conf.MemoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	conf.AspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	conf.CreateView = true;

	m_DepthAttachment->Init(m_State, conf);
}

void Cobalt::VulkanSwapchain::Recreate(int width, int height)
{
	Destroy();
	Create(width, height);
}

bool Cobalt::VulkanSwapchain::GetNextImageIndex(unsigned int timeoutMs, VkSemaphore imageAvailableSemaphore, VkFence fence, int* outImageIndex)
{
	{
		VkResult result = vkAcquireNextImageKHR(*m_State->Device, m_Swapchain, timeoutMs, imageAvailableSemaphore, fence, (uint32_t*)outImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			Recreate(m_State->FramebufferWidth, m_State->FramebufferHeight);
			return false;
		}
		if (!VK_CHECK(result))
			return false;
	}
	return true;
}

bool Cobalt::VulkanSwapchain::Present(VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, int presentImageIndex)
{
	VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderCompleteSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &m_Swapchain;
	presentInfo.pImageIndices = (uint32_t*)&presentImageIndex;
	presentInfo.pResults = 0;

	{
		VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			Recreate(m_State->FramebufferWidth, m_State->FramebufferHeight);
			return false;
		}
		if (!VK_CHECK(result))
			return false;
	}

	return true;
}

void Cobalt::VulkanSwapchain::Destroy()
{
	if (m_DepthAttachment)
		m_DepthAttachment->Shutdown();

	for (auto& i : m_ImageViews) {
		vkDestroyImageView(m_State->Device->GetLogicalDevice(), i, m_State->Allocator);
	}

	vkDestroySwapchainKHR(m_State->Device->GetLogicalDevice(), m_Swapchain, m_State->Allocator);
}
