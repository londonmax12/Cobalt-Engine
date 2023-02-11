#pragma once

#include "Application/Core.h"
#include "Math/Vector.h"
#include "Platforms/Vulkan/VulkanStructs.h"
#include "Platforms/Vulkan/Renderer/VulkanCommandBuffer.h"
#include "Platforms/Vulkan/Renderer/VulkanFramebuffer.h"

#include <vulkan/vulkan.h>

namespace Cobalt {
	enum RenderPassState {
		RENDER_PASS_READY,
		RENDER_PASS_RECORDING,
		RENDER_PASS_IN_PASS,
		RENDER_PASS_RECORDING_ENDED,
		RENDER_PASS_SUBMITTED,
		RENDER_PASS_NOT_ALLOCATED
	};

	class VulkanRenderPass {
	private:
		VkRenderPass m_RenderPass;

	public:
		bool Init(Ref<VulkanState> state, Vector2 renderPos, Vector2 renderSize, Color clearColor, float depth, int stencil);
		void Shutdown();

		void Begin(Ref<VulkanCommandBuffer> commandBuffer, VkFramebuffer framebuffer);
		void End(Ref<VulkanCommandBuffer> commandBuffer);

		VkRenderPass GetRenderPass() { return m_RenderPass; }

	private:
		Ref<VulkanState> m_State;

		Vector2 m_RenderPosition;
		Vector2 m_RenderSize;

		Color m_ClearColor;
		RenderPassState m_CurrentState;
		float m_Depth;
		int m_Stencil;
	};
}