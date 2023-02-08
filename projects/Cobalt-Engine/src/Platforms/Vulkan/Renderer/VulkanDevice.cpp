#include "cobaltpch.h"

#include "VulkanDevice.h"

#include "Platforms/Vulkan/Utility/VulkanUtility.h"

bool Cobalt::VulkanDevice::Init(Ref<VulkanState> state)
{
	m_State = state;
	m_SwapchainInfo = CreateRef<VulkanSwapchainSupportInfo>();

	uint32_t deviceCount = 0;
	if (!VK_CHECK(vkEnumeratePhysicalDevices(m_State->Instance, &deviceCount, 0))) {
		COBALT_FATAL("Failed to enumerate physical devices");
	}
	if (deviceCount == 0) {
		COBALT_FATAL("Failed to find supported Vulkan device");
	}
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_State->Instance, &deviceCount, devices.data());
	for (auto& device : devices) {
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);

		VkPhysicalDeviceMemoryProperties memory;
		vkGetPhysicalDeviceMemoryProperties(device, &memory);

		DeviceRequirements info{};
		info.Graphics = true;
		info.Present = true;
		info.Transfer = true;

		info.SamplerAnisomtropy = true;
		info.DiscreteGpu = true;
		info.Extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		QueueFamilyInfo queueInfo{};
		COBALT_INFO("Device Details ({})", properties.deviceName);
		bool result = DeviceMeetsRequirements(device, m_State->Surface, &properties, &features, info, &queueInfo, m_SwapchainInfo.get());
		if (result) {
			switch (properties.deviceType)
			{
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
				COBALT_INFO("\t| GPU type: Unknown");
				break;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				COBALT_INFO("\t| GPU type: Integrated");
				break;
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				COBALT_INFO("\t| GPU type: Discrete");
				break;
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				COBALT_INFO("\t| GPU type: Virtual");
				break;
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				COBALT_INFO("\t| GPU type: CPU");
				break;
			default:
				break;
			}
			COBALT_INFO("\t| GPU Driver Version: {}.{}.{}", VK_VERSION_MAJOR(properties.driverVersion), VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion));
			COBALT_INFO("\t| Vulkan API Version: {}.{}.{}", VK_VERSION_MAJOR(properties.apiVersion), VK_VERSION_MINOR(properties.apiVersion), VK_VERSION_PATCH(properties.apiVersion));
			for (int j = 0; j < (int)memory.memoryHeapCount; j++) {
				float memorySizeGB = ((float)memory.memoryHeaps[j].size) / 1024.f / 1024.f / 1024.f;
				if (memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
					COBALT_INFO("\t| Local GPU memory: {:.1f} GiB", memorySizeGB);
				}
				else
				{
					COBALT_INFO("\t| Shared system memory: {:.1f} GiB", memorySizeGB);
				}
			}
			COBALT_INFO("\t| Device ID: {}", properties.deviceID);
			m_PhysicalDevice = device;

			m_GraphicsIndex = queueInfo.GraphicsIndex;
			m_PresentIndex = queueInfo.PresentIndex;
			m_TransferIndex = queueInfo.TransferIndex;

			m_Properties = properties;
			m_Features = features;
			m_Memory = memory;

			break;
		}
	}

	if (!m_PhysicalDevice) {
		COBALT_ERROR("No physical device which meets requirements was found");
		return false;
	}

	bool presentSharesGraphicsQueue = m_GraphicsIndex == m_PresentIndex;
	bool transferSharesGraphicsQueue = m_GraphicsIndex == m_TransferIndex;
	int indexCount = 1;
	if (!presentSharesGraphicsQueue) {
		indexCount++;
	}
	if (!transferSharesGraphicsQueue) {
		indexCount++;
	}
	std::vector<int> indices(indexCount);
	int index = 0;
	indices[index++] = m_GraphicsIndex;
	if (!presentSharesGraphicsQueue) {
		indices[index++] = m_PresentIndex;
	}
	if (!transferSharesGraphicsQueue) {
		indices[index++] = m_TransferIndex;
	}
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfo(indexCount);
	for (int i = 0; i < indexCount; i++) {
		queueCreateInfo[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo[i].queueFamilyIndex = indices[i];
		queueCreateInfo[i].queueCount = 1;
		// TODO: Some devices don't support this
		//if (indices[i] == m_GraphicsIndex) {
		//	queueCreateInfo[i].queueCount = 2;
		//}
		queueCreateInfo[i].flags = 0;
		queueCreateInfo[i].pNext = nullptr;
		float queuePriority = 1.f;
		queueCreateInfo[i].pQueuePriorities = &queuePriority;
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = indexCount;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfo.data();
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
	deviceCreateInfo.enabledExtensionCount = 1;
	const char* extensionNames = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	deviceCreateInfo.ppEnabledExtensionNames = &extensionNames;

	if (!VK_CHECK(vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, m_State->Allocator, &m_LogicalDevice))) {
		COBALT_FATAL("Failed to create logical device");
		return false;
	}

	COBALT_INFO("Logical device created");

	if (!DetectDepthFormat())
	{
		COBALT_FATAL("Failed to detect Vulkan depth format");
		return false;
	}
	return true;
}

void Cobalt::VulkanDevice::Shutdown()
{
	if (m_LogicalDevice)
		vkDestroyDevice(m_LogicalDevice, m_State->Allocator);
}

void Cobalt::VulkanDevice::RequerySwapchainSupport()
{
	QuerySwapchainSupportInfo(m_PhysicalDevice, m_State->Surface, m_SwapchainInfo.get());
}

bool Cobalt::VulkanDevice::DeviceMeetsRequirements(VkPhysicalDevice device, VkSurfaceKHR surface, const VkPhysicalDeviceProperties* properties, const VkPhysicalDeviceFeatures* features, const DeviceRequirements requirements, QueueFamilyInfo* outQueueFamilyInfo, VulkanSwapchainSupportInfo* outSwapchainSupport)
{
	outQueueFamilyInfo->GraphicsIndex = -1;
	outQueueFamilyInfo->PresentIndex = -1;
	outQueueFamilyInfo->ComputeIndex = -1;
	outQueueFamilyInfo->TransferIndex = -1;

	if (requirements.DiscreteGpu) {
		if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			COBALT_INFO("Device is not a discrete GPU");
			return false;
		}
	}

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, 0);
	VkQueueFamilyProperties* queueFamilies = new VkQueueFamilyProperties[queueFamilyCount];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

	int minTransferScore = 255;
	for (int i = 0; i < (int)queueFamilyCount; i++) {
		int transferScore = 0;
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			outQueueFamilyInfo->GraphicsIndex = i;
			transferScore++;
		}
		if (queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
			outQueueFamilyInfo->ComputeIndex = i;
			transferScore++;
		}
		if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
			if (transferScore <= minTransferScore) {
				minTransferScore = transferScore;
				outQueueFamilyInfo->TransferIndex = i;
			}
		}
		VkBool32 supportsPresent = VK_FALSE;
		if (!VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &supportsPresent))) {
			COBALT_FATAL("Failed to get Vulkan physical device surface support");
			return false;
		}
		if (supportsPresent) {
			outQueueFamilyInfo->PresentIndex = i;
		}
		COBALT_INFO("\t| Queue Support: Graphics({}), Present({}), Compute({}), Transfer({})", (int)(outQueueFamilyInfo->GraphicsIndex != -1), (int)(outQueueFamilyInfo->PresentIndex != -1), (int)(outQueueFamilyInfo->ComputeIndex != -1), (int)(outQueueFamilyInfo->TransferIndex != -1));
		if (
			!requirements.Graphics || outQueueFamilyInfo->GraphicsIndex != -1 &&
			!requirements.Present || outQueueFamilyInfo->PresentIndex != -1 &&
			!requirements.Compute || outQueueFamilyInfo->ComputeIndex != -1 &&
			!requirements.Transfer || outQueueFamilyInfo->TransferIndex != -1)
		{

			QuerySwapchainSupportInfo(device, surface, outSwapchainSupport);

			if (outSwapchainSupport->Formats.size() < 1 || outSwapchainSupport->PresentModes.size() < 1) {
				COBALT_INFO("Device does not meet swapchain requirements");
				return false;
			}
			int reqExtensions = (int)requirements.Extensions.size();
			if (reqExtensions > 0) {
				uint32_t count;
				if (!VK_CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr))) {
					COBALT_ERROR("Failed to enumerate device extension properties");
					return false;
				}
				std::vector<VkExtensionProperties> extensions(count);
				vkEnumerateDeviceExtensionProperties(device, nullptr, &count, extensions.data());

				for (int i = 0; i < reqExtensions; i++) {
					bool found = false;
					for (int j = 0; j < (int)count; j++) {
						if (requirements.Extensions[i] == (std::string)extensions[j].extensionName) {
							found = true;
							break;
						}
					}
					if (!found) {
						COBALT_INFO("Device missing required extension: {}", requirements.Extensions[i].c_str());
						return false;
					}
				}
			}
		}
		if (requirements.SamplerAnisomtropy && !features->samplerAnisotropy) {
			COBALT_INFO("Device does not support SamplerAnisotropy");
			return false;
		}
		return true;
	}
	delete[] queueFamilies;
	return false;
}

void Cobalt::VulkanDevice::QuerySwapchainSupportInfo(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VulkanSwapchainSupportInfo* outInfo)
{
	if (!VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &outInfo->Capibilities))) {
		COBALT_ERROR("Failed to get Vulkan physical device surface capibilities");
		return;
	}
	uint32_t formatCount = 0;
	if (!VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, 0))) {
		COBALT_ERROR("Failed to get Vulkan physical device surface formats");
		return;
	}
	if (formatCount != 0) {
		outInfo->Formats = std::vector<VkSurfaceFormatKHR>(formatCount);
		VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, outInfo->Formats.data()));
	}
	uint32_t presentModeCount = 0;
	if (!VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, 0))) {
		COBALT_ERROR("Failed to get Vulkan physical device surface present modes");
		return;
	}
	if (presentModeCount != 0) {
		outInfo->PresentModes = std::vector<VkPresentModeKHR>(presentModeCount);
		VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, outInfo->PresentModes.data()));
	}
}

bool Cobalt::VulkanDevice::DetectDepthFormat()
{
	std::vector<VkFormat> candidates;
	candidates.push_back(VK_FORMAT_D32_SFLOAT);
	candidates.push_back(VK_FORMAT_D32_SFLOAT_S8_UINT);
	candidates.push_back(VK_FORMAT_D24_UNORM_S8_UINT);

	int flags = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

	for (auto& candidate : candidates) {
		VkFormatProperties properties{};
		vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice, candidate, &properties);

		if ((properties.linearTilingFeatures & flags) == flags) {
			m_DepthFormat = candidate;
			return true;
		}
		if ((properties.optimalTilingFeatures & flags) == flags) {
			m_DepthFormat = candidate;
			return true;
		}
	}
	return false;
}
