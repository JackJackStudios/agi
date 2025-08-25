#include "agipch.hpp"
#include "VulkanRenderPass.hpp"

#include "VulkanRenderContext.hpp"

namespace AGI {

	bool VulkanRenderPass::Create(VulkanContext* context, const RenderPassSpecification& spec)
	{
		m_Specification = spec;
		m_Context = context;

		if (m_Context == nullptr)
		{
			AGI_ERROR("VulkanRenderPass cannot be created with no context attached");
			return false;
		}

		if (spec.RenderArea.z == 0 || spec.RenderArea.w == 0)
		{
			auto window = m_Context->GetBoundWindow();
			AGI_WARN("Set render area to 0. Using window size ({}, {})", window->GetWidth(), window->GetHeight());

			m_Specification.RenderArea = { spec.RenderArea.x, spec.RenderArea.y, window->GetWidth(), window->GetHeight() };
		}

		// Currently only using one subpass
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		// Attachments for renderpass
		VkAttachmentDescription colourAttachment = {};
		colourAttachment.format = m_Specification.ColourFormat;
		colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		colourAttachment.flags = 0;

		VkAttachmentReference colourAttachmentRef;
		colourAttachmentRef.attachment = 0;
		colourAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colourAttachmentRef;

		VkSubpassDependency dependency;
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dependencyFlags = 0;

		VkRenderPassCreateInfo createInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &colourAttachment;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;
		createInfo.flags = 0;
		
		VK_CHECK_RETURN(vkCreateRenderPass, m_Context->GetDevice().Logical, &createInfo, m_Context->GetAllocator(), &m_RendererID);

		return true;
	}

	void VulkanRenderPass::Destroy()
	{
		if (m_RendererID)
		{
			vkDestroyRenderPass(m_Context->GetDevice().Logical, m_RendererID, m_Context->GetAllocator());
			m_RendererID == nullptr;
		}
	}

	void VulkanRenderPass::Begin(VulkanCommandBuffer& command, VkFramebuffer framebuffer)
	{
		VkRenderPassBeginInfo beginInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
		beginInfo.renderPass = m_RendererID;
		beginInfo.framebuffer = framebuffer;
		beginInfo.renderArea.offset.x = m_Specification.RenderArea.x;
		beginInfo.renderArea.offset.y = m_Specification.RenderArea.y;
		beginInfo.renderArea.extent.width = m_Specification.RenderArea.z;
		beginInfo.renderArea.extent.height = m_Specification.RenderArea.w;

		VkClearValue clearColour = {};
		clearColour.color.float32[0] = m_Specification.ClearColor.x;
		clearColour.color.float32[1] = m_Specification.ClearColor.y;
		clearColour.color.float32[2] = m_Specification.ClearColor.z;
		clearColour.color.float32[3] = m_Specification.ClearColor.w;

		beginInfo.clearValueCount = 1;
		beginInfo.pClearValues = &clearColour;

		vkCmdBeginRenderPass(command.GetHandle(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
		m_CurrentCommands = &command;
	}

	void VulkanRenderPass::End()
	{
		vkCmdEndRenderPass(m_CurrentCommands->GetHandle());
	}

};