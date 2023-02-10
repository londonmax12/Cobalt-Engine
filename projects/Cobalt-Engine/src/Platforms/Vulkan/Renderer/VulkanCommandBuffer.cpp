#include "cobaltpch.h"

#include "VulkanCommandBuffer.h"

#include "Platforms/Vulkan/Utility/VulkanUtility.h"
#include "Platforms/Vulkan/Renderer/VulkanDevice.h"
#include "Platforms/Vulkan/Renderer/VulkanCommandBuffer.h"

bool Cobalt::VulkanCommandBuffer::Allocate(Ref<VulkanState> state, VkCommandPool pool, bool isPrimary)
{
	m_State = state;

	if (m_CurrentState != COMMAND_BUFFER_NOT_ALLOCATED) {
		COBALT_WARN("Command buffer already allocated");
		return true;
	}

	VkCommandBufferAllocateInfo commandBufferCreateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
	commandBufferCreateInfo.commandPool = pool;
	commandBufferCreateInfo.level = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
	commandBufferCreateInfo.commandBufferCount = 1;

	{
		VkResult result = vkAllocateCommandBuffers(m_State->Device->GetLogicalDevice(), &commandBufferCreateInfo, &m_CommandBuffer);
		if (!VK_CHECK(result)) return false;
	}

	m_CurrentState = COMMAND_BUFFER_READY;
}

void Cobalt::VulkanCommandBuffer::Free(VkCommandPool pool)
{
	if (!m_CommandBuffer)
		return;

	vkFreeCommandBuffers(m_State->Device->GetLogicalDevice(), pool, 1, &m_CommandBuffer);
	m_CommandBuffer = 0;
	m_CurrentState = COMMAND_BUFFER_NOT_ALLOCATED;
}

bool Cobalt::VulkanCommandBuffer::AllocateBeginSingleUse(Ref<VulkanState> state, VkCommandPool pool)
{
	if (!Allocate(m_State, pool, true))
	{
		COBALT_ERROR("Failed to allocate single use command buffer");
		return false;
	}
	Begin(true, false, false);
	return true;
}

void Cobalt::VulkanCommandBuffer::EndSingleUse(VkCommandPool pool, VkQueue queue)
{
	End();

	VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_CommandBuffer;

	VK_CHECK(vkQueueSubmit(queue, 1, &submitInfo, 0));
	VK_CHECK(vkQueueWaitIdle(queue));

	Free(pool);
}

void Cobalt::VulkanCommandBuffer::Begin(bool isSingleUse, bool isRenderPassContinue, bool isSimultaneous)
{
	VkCommandBufferBeginInfo commandBufferBeginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

	if (isSingleUse)			commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	if (isSimultaneous)			commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	if (isRenderPassContinue)	commandBufferBeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;

	VK_CHECK(vkBeginCommandBuffer(m_CommandBuffer, &commandBufferBeginInfo));
	m_CurrentState = COMMAND_BUFFER_RECORDING;
}

void Cobalt::VulkanCommandBuffer::End()
{
	if (m_CurrentState != COMMAND_BUFFER_RECORDING) {
		COBALT_WARN("Command buffer not in correct state to end");
		return;
	}

	VK_CHECK(vkEndCommandBuffer(m_CommandBuffer));
}

void Cobalt::VulkanCommandBuffer::Reset()
{
	if (m_CurrentState == COMMAND_BUFFER_NOT_ALLOCATED)
		return;

	m_CurrentState = COMMAND_BUFFER_READY;
}
