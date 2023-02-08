#pragma once

#include "Application/Core.h"
#include "Platforms/Vulkan/VulkanStructs.h"

#include <vulkan/vulkan.h>

namespace Cobalt {
	struct VulkanImageConfig
	{
		int Width;
		int Height;
		VkImageType ImageType;
		VkFormat Format;
		VkImageTiling Tiling;
		VkImageUsageFlags UsageFlags;
		VkMemoryPropertyFlags MemoryFlags;
		VkImageAspectFlags AspectFlags;
		bool CreateView;
	};

	class VulkanImage {
	public:
		bool Init(Ref<VulkanState> state, VulkanImageConfig config);
		void Shutdown();

		void CreateView(VkFormat format,  VkImageAspectFlags aspectFlags);
	private:
		VkImage m_Image{};
		VkDeviceMemory m_Memory{};
		VkImageView m_View{};
		int m_Width;
		int m_Height;

		Ref<VulkanState> m_State;
	};
}