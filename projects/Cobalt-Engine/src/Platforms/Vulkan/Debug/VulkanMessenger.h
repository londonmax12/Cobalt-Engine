#pragma once

#include "Application/Core.h"
#include "Platforms/Vulkan/VulkanStructs.h"

#include <vulkan/vulkan.h>

namespace Cobalt {
	class VulkanMessenger
	{
	public:
		bool Init(Ref<VulkanState> state);
		void Shutdown();
	private:
		VkDebugUtilsMessengerEXT m_Messenger = nullptr;
		Ref<VulkanState> m_State;
	};


}
