#include "cobaltpch.h"

#include "VulkanRenderPass.h"

#include "Platforms/Vulkan/Renderer/VulkanSwapchain.h"
#include "Platforms/Vulkan/Renderer/VulkanDevice.h"
#include "Platforms/Vulkan/Utility/VulkanUtility.h"

bool Cobalt::VulkanRenderPass::Init(Ref<VulkanState> state, Vector2 renderPos, Vector2 renderSize, Color clearColor, float depth, int stencil)
{
    m_State = state;
    m_RenderPosition = renderPos;
    m_RenderSize = renderSize;
    m_ClearColor = clearColor;
    m_Depth = depth;
    m_Stencil = stencil;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

    std::vector<VkAttachmentDescription> attachmentDescriptions;

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format =  m_State->Swapchain->GetFormat().format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    colorAttachment.flags = 0;

    attachmentDescriptions.push_back(colorAttachment);

    VkAttachmentReference colorAttachmentReference{};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentReference;

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = m_State->Device->GetDepthFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachmentDescriptions.push_back(depthAttachment);

    VkAttachmentReference depthAttachmentReference{};
    depthAttachmentReference.attachment = 1;
    depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    subpass.pDepthStencilAttachment = &depthAttachmentReference;


    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = 0;

    subpass.pResolveAttachments = 0;

    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = 0;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependency.dependencyFlags = 0;

    VkRenderPassCreateInfo renderpassCreateInfo{};
    renderpassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderpassCreateInfo.attachmentCount = attachmentDescriptions.size();
    renderpassCreateInfo.pAttachments = attachmentDescriptions.data();
    renderpassCreateInfo.subpassCount = 1;
    renderpassCreateInfo.pSubpasses = &subpass;
    renderpassCreateInfo.dependencyCount = 1;
    renderpassCreateInfo.pDependencies = &dependency;

    {
        VkResult result = vkCreateRenderPass(m_State->Device->GetLogicalDevice(), &renderpassCreateInfo, m_State->Allocator, &m_RenderPass);
        if (!VK_CHECK(result)) return false;
    }

    return true;
}

void Cobalt::VulkanRenderPass::Shutdown()
{
    if (m_RenderPass) {
        vkDestroyRenderPass(m_State->Device->GetLogicalDevice(), m_RenderPass, m_State->Allocator);
    }
}

void Cobalt::VulkanRenderPass::Begin(Ref<VulkanCommandBuffer> commandBuffer, VkFramebuffer framebuffer)
{
    VkRenderPassBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.renderPass = m_RenderPass;
    beginInfo.framebuffer = framebuffer;
    beginInfo.renderArea.offset.x = m_RenderPosition.x;
    beginInfo.renderArea.offset.y = m_RenderPosition.y;
    beginInfo.renderArea.extent.width = m_RenderSize.x;
    beginInfo.renderArea.extent.height = m_RenderSize.y;

    std::vector<VkClearValue> clearValues(2);

    clearValues[0].color.float32[0] = m_ClearColor.r;
    clearValues[0].color.float32[1] = m_ClearColor.g;
    clearValues[0].color.float32[2] = m_ClearColor.b;
    clearValues[0].color.float32[3] = m_ClearColor.a;

    clearValues[1].depthStencil.depth = m_Depth;
    clearValues[1].depthStencil.stencil = m_Stencil;

    beginInfo.clearValueCount = 2;
    beginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer->GetCommandBuffer(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
    commandBuffer->SetState(COMMAND_BUFFER_IN_PASS);
}

void Cobalt::VulkanRenderPass::End(Ref<VulkanCommandBuffer> commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer->GetCommandBuffer());
    commandBuffer->SetState(COMMAND_BUFFER_RECORDING);
}
