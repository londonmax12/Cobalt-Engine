#pragma once

#include "Platforms/Vulkan/VulkanStructs.h"
#include "Application/Core.h"

namespace Cobalt {
	class VulkanFramebuffer {
	private:
		VkFramebuffer m_Framebuffer;
	public:
		bool Init(Ref<VulkanState> state, Ref<VulkanRenderPass> renderPass, int width, int height, std::vector<VkImageView> attachments);
		void Shutdown();

		VkFramebuffer GetFramebuffer() { return m_Framebuffer; }
	private:
		Ref<VulkanRenderPass> m_RenderPass;
		Ref<VulkanState> m_State;
		std::vector<VkImageView> m_Attachments;
	};
}