#include "cobaltpch.h"

#include "Platforms/Vulkan/VulkanPlatform.h"

#include "Platforms/Windows/WindowsStructs.h"
#include "Platforms/Vulkan/VulkanStructs.h"
#include "Platforms/Vulkan/Utility/VulkanUtility.h" 

#include <vulkan/vulkan_win32.h>

#ifdef COBALT_PLATFORM_WINDOWS
void Cobalt::Platform::GetVulkanExtensions(std::vector<const char*>* out)
{
	out->push_back("VK_KHR_win32_surface");
}

VkSurfaceKHR Cobalt::Platform::CreateVulkanSurface(PlatformState* platformState, Ref<VulkanState> vulkanState) {
	WindowsInternalState* state = (WindowsInternalState*)platformState;

	if (!state)
	{
		COBALT_FATAL("Failed to create Win32 Vulkan surface: State is invalid");
		return NULL;
	}
	if (!state->hwnd)
	{
		COBALT_FATAL("Failed to create Win32 Vulkan surface: hwnd is invalid");
		return NULL;
	}
	if (!vulkanState->Instance) {
		COBALT_FATAL("Failed to create Win32 Vulkan surface: Vulkan instance is not set");
		return NULL;
	}

	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };

	surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
	surfaceCreateInfo.hwnd = state->hwnd;

	VkSurfaceKHR newSurface;
	if (!VK_CHECK(vkCreateWin32SurfaceKHR(vulkanState->Instance, &surfaceCreateInfo, vulkanState->Allocator, &newSurface)))
	{
		COBALT_FATAL("Failed to create Win32 Vulkan surface");
		return NULL;
	}

	COBALT_INFO("Created Win32 Vulkan Surface");

	return newSurface;
}
#endif