#pragma once

#include <vulkan/vulkan.h>

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
		CommandBufferState m_CurrentState;

	public:
		bool Init();
		void Shutdown();

		VkCommandBuffer GetCommandBuffer() { return m_CommandBuffer; }

		void SetState(CommandBufferState state) { m_CurrentState = state; }
	private:
	};
}