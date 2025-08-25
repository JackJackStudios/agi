#pragma once
#include "Vulkan.hpp"

#include "VulkanCommandBuffer.hpp"

namespace AGI {

	struct RenderPassSpecification
	{
		// x, y, width, height
		glm::vec4 RenderArea;
		glm::vec4 ClearColor;

		VkFormat ColourFormat;

		// TODO: Depth and stencil buffers
	};

	class VulkanContext;

	class VulkanRenderPass
	{
	public:
		enum class State
		{
			Ready = 0, Recording, InPass, RecordingEnded, Submitted, NotAllocated,
		};
		VulkanRenderPass() = default;

		bool Create(VulkanContext* context, const RenderPassSpecification& spec);
		void Destroy();

		// Setters and getters
		void SetClearColour(const glm::vec4& colour) { m_Specification.ClearColor = colour; }

		// Changes state
		void Begin(VulkanCommandBuffer& command, VkFramebuffer framebuffer);
		void End();
	private:
		VkRenderPass m_RendererID = nullptr;
		VulkanCommandBuffer* m_CurrentCommands = nullptr; // Set in Begin()

		RenderPassSpecification m_Specification;
		State m_CurrentState = State::NotAllocated;

		VulkanContext* m_Context;
	};

};