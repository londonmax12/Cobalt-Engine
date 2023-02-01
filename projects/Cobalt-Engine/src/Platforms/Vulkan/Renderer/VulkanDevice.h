#pragma once

#include "Platforms/Vulkan/VulkanStructs.h"

#include <vulkan/vulkan.h>

namespace Cobalt {
	struct DeviceRequirements {
		bool Graphics;
		bool Present;
		bool Compute;
		bool Transfer;

		std::vector<std::string> Extensions;
		bool SamplerAnisomtropy;
		bool DiscreteGpu;
	};

	struct QueueFamilyInfo {
		int GraphicsIndex;
		int PresentIndex;
		int ComputeIndex;
		int TransferIndex;
	};

	class VulkanDevice {
	private:
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_LogicalDevice;

	public:
		bool Init(VkInstance instance, VkSurfaceKHR surface);

		operator VkDevice() const { return m_LogicalDevice; } 
		operator VkPhysicalDevice() const { return m_PhysicalDevice; }
	private:
		bool DeviceMeetsRequirements(VkPhysicalDevice device,
			VkSurfaceKHR surface,
			const VkPhysicalDeviceProperties* properties,
			const VkPhysicalDeviceFeatures* features,
			const DeviceRequirements requirements,
			QueueFamilyInfo* outQueueFamilyInfo,
			VulkanSwapchainSupportInfo* outSwapchainSupport);

		void QuerySwapchainSupportInfo(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VulkanSwapchainSupportInfo* outInfo);

		VkInstance m_Instance;
		VulkanSwapchainSupportInfo m_SwapchainInfo;

		int m_GraphicsIndex;
		int m_PresentIndex;
		int m_TransferIndex;

		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDeviceMemoryProperties m_Memory;
	};
}