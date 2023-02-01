#include "cobaltpch.h"

#include "VulkanDevice.h"

#include "Platforms/Vulkan/Utility/VulkanUtility.h"

bool Cobalt::VulkanDevice::Init(VkInstance instance, VkSurfaceKHR surface, VkAllocationCallbacks* allocator)
{
	m_Instance = instance;
	m_Allocator = allocator;

	uint32_t deviceCount = 0;
	if (!VK_CHECK(vkEnumeratePhysicalDevices(instance, &deviceCount, 0))) {
		COBALT_FATAL("Failed to enumerate physical devices");
	}
	if (deviceCount == 0) {
		COBALT_FATAL("Failed to find supported Vulkan device");
	}
	VkPhysicalDevice* a = new VkPhysicalDevice[deviceCount];
	vkEnumeratePhysicalDevices(instance, &deviceCount, a);
	for (int i = 0; i < deviceCount; i++) {

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(a[i], &properties);

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(a[i], &features);

		VkPhysicalDeviceMemoryProperties memory;
		vkGetPhysicalDeviceMemoryProperties(a[i], &memory);

		DeviceRequirements info{};
		info.Graphics = true;
		info.Present = true;
		info.Transfer = true;

		info.SamplerAnisomtropy = true;
		info.DiscreteGpu = true;
		info.Extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		QueueFamilyInfo queueInfo{};
		bool result = DeviceMeetsRequirements(a[i], surface, &properties, &features, info, &queueInfo, &m_SwapchainInfo);
		if (result) {
			COBALT_INFO("------------------------------------------");
			COBALT_INFO(properties.deviceName);
			COBALT_INFO("");
			switch (properties.deviceType)
			{
			case VK_PHYSICAL_DEVICE_TYPE_OTHER:
				COBALT_INFO("\tGPU type: Unknown");
				break;
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
				COBALT_INFO("\tGPU type: Integrated");
				break;
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
				COBALT_INFO("\tGPU type: Discrete");
				break;
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
				COBALT_INFO("\tGPU type: Virtual");
				break;
			case VK_PHYSICAL_DEVICE_TYPE_CPU:
				COBALT_INFO("\tGPU type: CPU");
				break;
			default:
				break;
			}
			COBALT_INFO("\tGPU Driver Version: {}.{}.{}", VK_VERSION_MAJOR(properties.driverVersion), VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion));
			COBALT_INFO("\tVulkan API Version: {}.{}.{}", VK_VERSION_MAJOR(properties.apiVersion), VK_VERSION_MINOR(properties.apiVersion), VK_VERSION_PATCH(properties.apiVersion));
			COBALT_INFO("");
			for (int j = 0; j < memory.memoryHeapCount; j++) {
				float memorySizeGB = ((float)memory.memoryHeaps[j].size) / 1024.f / 1024.f / 1024.f;
				if (memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
					COBALT_INFO("\tLocal GPU memory: {} GiB", memorySizeGB);
				}
				else
				{
					COBALT_INFO("\tShared system memory: {} GiB", memorySizeGB);
				}
			}
			COBALT_INFO("------------------------------------------");
			m_PhysicalDevice = a[i];

			m_GraphicsIndex = queueInfo.GraphicsIndex;
			m_PresentIndex = queueInfo.PresentIndex;
			m_TransferIndex = queueInfo.TransferIndex;

			m_Properties = properties;
			m_Features = features;
			m_Memory = memory;

			break;
		}
		COBALT_INFO("Selected device: {}", properties.deviceName);
	}

	if (!m_PhysicalDevice) {
		COBALT_ERROR("No physical device which meets requirements was found");
		return false;
	}
	delete[] a;

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
		if (indices[i] == m_GraphicsIndex) {
			queueCreateInfo[i].queueCount = 2;
		}
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

	if (!VK_CHECK(vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, allocator, &m_LogicalDevice))) {
		COBALT_FATAL("Failed to create logical device");
		return false;
	}

	COBALT_INFO("Logical device created");
	return true;
}

void Cobalt::VulkanDevice::Shutdown()
{
	if (m_LogicalDevice)
		vkDestroyDevice(m_LogicalDevice, m_Allocator);
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

	COBALT_INFO("Graphics | Present | Compute | Transfer | Name");
	int minTransferScore = 255;
	for (int i = 0; i < queueFamilyCount; i++) {
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
		COBALT_INFO("       {} |       {} |       {} |        {} | {}", (int)(outQueueFamilyInfo->GraphicsIndex != -1), (int)(outQueueFamilyInfo->PresentIndex != -1), (int)(outQueueFamilyInfo->ComputeIndex != -1), (int)(outQueueFamilyInfo->TransferIndex != -1), properties->deviceName);
		if (
			!requirements.Graphics || outQueueFamilyInfo->GraphicsIndex != -1 &&
			!requirements.Present || outQueueFamilyInfo->PresentIndex != -1 &&
			!requirements.Compute || outQueueFamilyInfo->ComputeIndex != -1 &&
			!requirements.Transfer || outQueueFamilyInfo->TransferIndex != -1)
		{
			COBALT_INFO("Device meets queue requirements");

			QuerySwapchainSupportInfo(device, surface, outSwapchainSupport);

			if (outSwapchainSupport->FormatCount < 1 || outSwapchainSupport->PresentModeCount < 1) {
				COBALT_INFO("Device does not meet swapchain requirements");
				return false;
			}
			int reqExtensions = requirements.Extensions.size();
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
					for (int j = 0; j < count; j++) {
						if (requirements.Extensions[i] == extensions[j].extensionName) {
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
	if (!VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &outInfo->FormatCount, outInfo->Formats))) {
		COBALT_ERROR("Failed to get Vulkan physical device surface formats");
		return;
	}
	if (outInfo->FormatCount != 0) {
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &outInfo->FormatCount, outInfo->Formats);
	}
	if (!VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &outInfo->PresentModeCount, 0))) {
		COBALT_ERROR("Failed to get Vulkan physical device surface present modes");
		return;
	}
	if (outInfo->PresentModeCount != 0) {
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &outInfo->PresentModeCount, outInfo->PresentModes);
	}
}