#include "cobaltpch.h"
#include "VulkanFramebuffer.h"

#include "Platforms/Vulkan/Renderer/VulkanDevice.h"
#include "Platforms/Vulkan/Renderer/VulkanRenderPass.h"
#include "Platforms/Vulkan/Utility/VulkanUtility.h"

bool Cobalt::VulkanFramebuffer::Init(Ref<VulkanState> state, Ref<VulkanRenderPass> renderPass, int width, int height, std::vector<VkImageView> attachments)
{
    m_State = state;
    m_RenderPass = renderPass;
    m_Attachments = attachments;

    VkFramebufferCreateInfo framebufferCreateInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
    framebufferCreateInfo.renderPass = m_RenderPass->GetRenderPass();
    framebufferCreateInfo.attachmentCount = (uint32_t)m_Attachments.size();
    framebufferCreateInfo.pAttachments = m_Attachments.data();
    framebufferCreateInfo.width = width;
    framebufferCreateInfo.height = height;
    framebufferCreateInfo.layers = 1;

    {
        VkResult result = vkCreateFramebuffer(m_State->Device->GetLogicalDevice(), &framebufferCreateInfo, m_State->Allocator, &m_Framebuffer);
        if (!VK_CHECK(result)) return false;
    }
    return true;
}

void Cobalt::VulkanFramebuffer::Shutdown()
{
    if (m_Framebuffer)
        vkDestroyFramebuffer(m_State->Device->GetLogicalDevice(), m_Framebuffer, m_State->Allocator);
}
