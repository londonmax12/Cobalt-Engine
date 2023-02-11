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
	private:
		VkImageView m_View{};

	public:
		bool Init(Ref<VulkanState> state, VulkanImageConfig config);
		void Shutdown();

		void CreateView(VkFormat format,  VkImageAspectFlags aspectFlags);

		VkImageView GetView() { return m_View; }
	private:
		VkImage m_Image{};
		VkDeviceMemory m_Memory{};

		int m_Width;
		int m_Height;

		Ref<VulkanState> m_State;
	};
}