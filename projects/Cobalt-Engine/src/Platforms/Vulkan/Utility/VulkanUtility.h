#pragma once

#include <vulkan/vulkan.h>

#define VK_CHECK(result) Cobalt::Utility::VulkanCheckResult(result, __LINE__, __FILE__)

namespace Cobalt {
	namespace Utility {
		enum VulkanResult {
			VulkanResultError = 0,
			VulkanResultWarning = 1,
			VulkanResultSuccess = 2,
		};

		VulkanResult VulkanSuccess(VkResult result);
		const char* VulkanResultToString(VkResult result, bool getExtended);

		bool VulkanCheckResult(VkResult result, int line, const char* file);
	}
}