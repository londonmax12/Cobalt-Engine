#pragma once
#include <vulkan/vulkan.h>

#include "Platforms/Vulkan/VulkanStructs.h"

namespace Cobalt {
	class VulkanFence {
	private:
		VkFence m_Fence;

	public:
		bool Init(Ref<VulkanState> state, bool createSignaled = false);
		void Shutdown();

		bool Wait(uint32_t timeoutMs);

		void Reset();

		operator VkFence() const { return m_Fence; }

	private:
		bool m_Signaled;
		Ref<VulkanState> m_State;
	};
}