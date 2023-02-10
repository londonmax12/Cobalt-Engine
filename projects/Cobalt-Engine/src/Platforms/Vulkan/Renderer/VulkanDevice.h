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

		VkFormat m_DepthFormat = VK_FORMAT_UNDEFINED;

		VkCommandPool m_GraphicsCommandPool;
	public:
		bool Init(Ref<VulkanState> state);
		void Shutdown();

		operator VkDevice() const { return m_LogicalDevice; }
		operator VkPhysicalDevice() const { return m_PhysicalDevice; }

		VkDevice const GetLogicalDevice() { return m_LogicalDevice; }
		VkPhysicalDevice const GetPhysicalDevice() { return m_PhysicalDevice; }

		Ref<VulkanSwapchainSupportInfo> GetSwapchainSupportInfo() { return m_SwapchainInfo; }
		
		VkCommandPool GetGraphicsCommandPool() { return m_GraphicsCommandPool; }

		void RequerySwapchainSupport();

		int GetGraphicsQueueIndex() { return m_GraphicsIndex; }
		int GetPresentQueueIndex() { return m_PresentIndex; }
		int GetTransferQueueIndex() { return m_TransferIndex; }

		VkFormat GetDepthFormat() { return m_DepthFormat; }
	private:
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkQueue m_TransferQueue;

		bool DeviceMeetsRequirements(VkPhysicalDevice device,
			VkSurfaceKHR surface,
			const VkPhysicalDeviceProperties* properties,
			const VkPhysicalDeviceFeatures* features,
			const DeviceRequirements requirements,
			QueueFamilyInfo* outQueueFamilyInfo,
			VulkanSwapchainSupportInfo* outSwapchainSupport);

		void QuerySwapchainSupportInfo(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VulkanSwapchainSupportInfo* outInfo);

		bool DetectDepthFormat();;

		Ref<VulkanState> m_State;
		
		Ref<VulkanSwapchainSupportInfo> m_SwapchainInfo;

		int m_GraphicsIndex;
		int m_PresentIndex;
		int m_TransferIndex;

		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDeviceMemoryProperties m_Memory;
	};
}