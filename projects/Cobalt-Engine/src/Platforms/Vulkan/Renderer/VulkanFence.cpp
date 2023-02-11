#include "cobaltpch.h"

#include "VulkanFence.h"

#include "Platforms/Vulkan/Utility/VulkanUtility.h"
#include "Platforms/Vulkan/Renderer/VulkanDevice.h"

bool Cobalt::VulkanFence::Init(Ref<VulkanState> state, bool createSignaled)
{
    m_State = state;
    m_Signaled = createSignaled;

    VkFenceCreateInfo fenceCreateInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fenceCreateInfo.flags = m_Signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

    {
        VkResult result = vkCreateFence(m_State->Device->GetLogicalDevice(), &fenceCreateInfo, m_State->Allocator, &m_Fence);
        if (!VK_CHECK(result)) return false;
    }
    return true;
}

void Cobalt::VulkanFence::Shutdown()
{
    if (m_Fence) {
        vkDestroyFence(m_State->Device->GetLogicalDevice(), m_Fence, m_State->Allocator);
    }

    m_Signaled = false;
}

bool Cobalt::VulkanFence::Wait(uint32_t timeoutMs)
{
    if (!m_Signaled)
    {
        VkResult result = vkWaitForFences(m_State->Device->GetLogicalDevice(), 1, &m_Fence, true, timeoutMs);
        bool success = VK_CHECK(result);

        if (result == VK_TIMEOUT) {
            COBALT_WARN("Vulkan fence timed out");
        }

        return success;
    }

    return true;
}

void Cobalt::VulkanFence::Reset()
{
    if (m_Signaled) {
        VK_CHECK(vkResetFences(m_State->Device->GetLogicalDevice(), 1, &m_Fence));
        m_Signaled = false;
    }
}
