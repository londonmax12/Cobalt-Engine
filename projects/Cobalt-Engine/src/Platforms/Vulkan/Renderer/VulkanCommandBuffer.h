#pragma once

#include <vulkan/vulkan.h>

#include "Platforms/Vulkan/VulkanStructs.h"
#include "Application/Core.h"

namespace Cobalt {
	enum CommandBufferState{
		COMMAND_BUFFER_READY,
		COMMAND_BUFFER_RECORDING,
		COMMAND_BUFFER_IN_PASS,
		COMMAND_BUFFER_RECORDING_ENDED,
		COMMAND_BUFFER_SUBMITTED,
		COMMAND_BUFFER_NOT_ALLOCATED
	};
	
	class VulkanCommandBuffer {
	private:
		VkCommandBuffer m_CommandBuffer;
		CommandBufferState m_CurrentState = COMMAND_BUFFER_NOT_ALLOCATED;
		Ref<VulkanState> m_State;

	public:
		VkCommandBuffer GetCommandBuffer() { return m_CommandBuffer; }

		void SetState(CommandBufferState state) { m_CurrentState = state; }

		bool Allocate(Ref<VulkanState> state, VkCommandPool pool, bool isPrimary);
		void Free(VkCommandPool pool);

		bool AllocateBeginSingleUse(Ref<VulkanState> state, VkCommandPool pool);
		void EndSingleUse(VkCommandPool pool, VkQueue queue);

		void Begin(bool isSingleUse, bool isRenderPassContinue, bool isSimultaneous);
		void End();

		void Reset();
	};
}