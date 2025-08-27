#include "agipch.hpp"
#include "VulkanCommandBuffer.hpp"

#include "VulkanRenderContext.hpp"

namespace AGI {

	bool VulkanCommandBuffer::Allocate(VulkanContext* context, VkCommandPool pool, bool is_primary)
	{
		m_BoundContext = context;
		m_Parent = pool;

		if (m_Parent == nullptr || m_BoundContext == nullptr)
		{
			AGI_ERROR("VulkanCommandBuffer cannot be created with no context or pool attached");
			return false;
		}

		VkCommandBufferAllocateInfo allocateInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocateInfo.commandPool = pool;
		allocateInfo.level = is_primary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY : VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		allocateInfo.commandBufferCount = 1;
		
		VK_CHECK_RETURN(vkAllocateCommandBuffers, m_BoundContext->GetDevice().Logical, &allocateInfo, &m_RendererID);
		m_CurrentState = State::Ready;
	}

	void VulkanCommandBuffer::Free()
	{
		if (m_RendererID)
		{
			vkFreeCommandBuffers(m_BoundContext->GetDevice().Logical, m_Parent, 1, &m_RendererID);
			m_RendererID = nullptr;
		}
	}

	void VulkanCommandBuffer::Begin(bool single_use, bool is_renderpass, bool simultaneous)
	{
		VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		if (single_use) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		if (is_renderpass) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		if (simultaneous) beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

		VK_CHECK(vkBeginCommandBuffer, m_RendererID, &beginInfo);
		m_CurrentState = State::Recording;
	}

	void VulkanCommandBuffer::End()
	{
		VK_CHECK(vkEndCommandBuffer, m_RendererID);
		m_CurrentState = State::RecordingEnded;
	}

	void VulkanCommandBuffer::UpdateSubmitted()
	{
		m_CurrentState = State::Submitted;
	}

	void VulkanCommandBuffer::Reset()
	{
		m_CurrentState = State::Ready;
	}

};