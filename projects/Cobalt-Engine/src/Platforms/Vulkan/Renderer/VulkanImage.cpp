#include "cobaltpch.h"

#include "VulkanImage.h"

#include "Platforms/Vulkan/Renderer/VulkanDevice.h"
#include "Platforms/Vulkan/Utility/VulkanUtility.h"
#include "Logging/Logging.h"

bool Cobalt::VulkanImage::Init(Ref<VulkanState> state, VulkanImageConfig config)
{
	m_State = state;
	m_Width = config.Width;
	m_Height = config.Height;

	VkImageCreateInfo imageCreateInfo{ VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.extent.width = m_Width;
	imageCreateInfo.extent.height = m_Height;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 4;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = config.Format;
	imageCreateInfo.tiling = config.Tiling;
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.usage = config.UsageFlags;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	{
		VkResult result = vkCreateImage(m_State->Device->GetLogicalDevice(), &imageCreateInfo, m_State->Allocator, &m_Image);
		if (!VK_CHECK(result)) return false;
	}

	VkMemoryRequirements memoryRequirements{};
	vkGetImageMemoryRequirements(m_State->Device->GetLogicalDevice(), m_Image, &memoryRequirements);
	
	int memoryType = VulkanFindMemoryIndex(memoryRequirements.memoryTypeBits, config.MemoryFlags, m_State->Device->GetPhysicalDevice());
	if (memoryType == -1) {
		COBALT_ERROR("Failed to create image: Memory type not found");
		return false;
	}

	VkMemoryAllocateInfo allocateInfo{};
	allocateInfo.allocationSize = memoryRequirements.size;
	allocateInfo.memoryTypeIndex = memoryType;

	{
		VkResult result = vkAllocateMemory(m_State->Device->GetLogicalDevice(), &allocateInfo, m_State->Allocator, &m_Memory);
		if (!VK_CHECK(result)) return false;
	}

	{
		VkResult result = vkBindImageMemory(m_State->Device->GetLogicalDevice(), m_Image, m_Memory, 0);
		if (!VK_CHECK(result)) return false;
	}

	if (config.CreateView)
	{
		m_View = 0;

	}

	return true;
}

void Cobalt::VulkanImage::Shutdown()
{
	if (m_View)
		vkDestroyImageView(m_State->Device->GetLogicalDevice(), m_View, m_State->Allocator);

	if (m_Memory)
		vkFreeMemory(m_State->Device->GetLogicalDevice(), m_Memory, m_State->Allocator);

	if (m_Image)
		vkDestroyImage(m_State->Device->GetLogicalDevice(), m_Image, m_State->Allocator);
}

void Cobalt::VulkanImage::CreateView(VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewCreationInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
	viewCreationInfo.image = m_Image;
	viewCreationInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewCreationInfo.format = format;
	viewCreationInfo.subresourceRange.aspectMask = aspectFlags;
	viewCreationInfo.subresourceRange.baseMipLevel = 0;
	viewCreationInfo.subresourceRange.levelCount = 1;
	viewCreationInfo.subresourceRange.baseArrayLayer = 0;
	viewCreationInfo.subresourceRange.layerCount = 1;

	{
		VkResult result = vkCreateImageView(m_State->Device->GetLogicalDevice(), &viewCreationInfo, m_State->Allocator, &m_View);
		if (!VK_CHECK(result)) return;
	}
}
