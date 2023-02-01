#pragma once

#include "Platforms/Vulkan/VulkanStructs.h"

#include <vulkan/vulkan.h>

namespace Cobalt {
	class VulkanDebugger
	{
	public:
		bool Init(Ref<VulkanState> state);
		void Shutdown();
	private:
		VkDebugUtilsMessengerEXT m_Messenger;
		Ref<VulkanState> m_State;
	};


}