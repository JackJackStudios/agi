#include "agipch.hpp"
#include "VulkanFramebuffer.hpp"

#include "VulkanRenderContext.hpp"

namespace AGI {

	bool VulkanFramebuffer::Create(VulkanContext* context, VulkanRenderPass* renderpass, const glm::vec2& size, const std::vector<VkImageView>& attachments)
	{
		m_BoundContext = context;
		m_Parent = renderpass;

		if (m_Parent == nullptr || m_BoundContext == nullptr)
		{
			AGI_ERROR("VulkanFramebuffer cannot be created with no context or renderpass attached");
			return false;
		}

		m_Attachments.resize(attachments.size());
		for (int i = 0; i < m_Attachments.size(); ++i)
			m_Attachments[i] = attachments[i];

		VkFramebufferCreateInfo createInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
		createInfo.renderPass = m_Parent->GetHandle();
		createInfo.attachmentCount = m_Attachments.size();
		createInfo.pAttachments = m_Attachments.data();
		createInfo.width = size.x;
		createInfo.height = size.y;
		createInfo.layers = 1;

		VK_CHECK_RETURN(vkCreateFramebuffer, m_BoundContext->GetDevice().Logical, &createInfo, m_BoundContext->GetAllocator(), &m_RendererID);
		return true;
	}

	void VulkanFramebuffer::Destroy()
	{
		if (m_RendererID)
		{
			vkDestroyFramebuffer(m_BoundContext->GetDevice().Logical, m_RendererID, m_BoundContext->GetAllocator());
			m_RendererID = nullptr;
		}
	}

};