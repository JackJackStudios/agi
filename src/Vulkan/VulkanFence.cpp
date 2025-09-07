#include "agipch.hpp"
#include "VulkanFence.hpp"

#include "VulkanRenderContext.hpp"

namespace AGI {

	bool VulkanFence::Create(VulkanContext* context, bool create_signaled)
	{
		m_IsSignaled = create_signaled;
		m_Context = context;

		if (m_Context == nullptr)
		{
			AGI_ERROR("VulkanFence cannot be created with no context attached");
			return false;
		}

		VkFenceCreateInfo createInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
		createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VK_CHECK_RETURN(vkCreateFence, m_Context->GetDevice().Logical, &createInfo, m_Context->GetAllocator(), &m_RendererID);
		return true;
	}

	void VulkanFence::Destroy()
	{
		if (m_RendererID)
		{
			vkDestroyFence(m_Context->GetDevice().Logical, m_RendererID, m_Context->GetAllocator());
			m_RendererID = nullptr;
		}
	}

	bool VulkanFence::Wait(uint64_t timeout)
	{
		if (m_IsSignaled) return true;

		VkResult result = vkWaitForFences(
			m_Context->GetDevice().Logical,
			1,
			&m_RendererID,
			true,
			timeout);

		switch (result)
		{
		case VK_SUCCESS:
			m_IsSignaled = true;
			return true;

		case VK_TIMEOUT: return false;
		case VK_ERROR_DEVICE_LOST:
		case VK_ERROR_OUT_OF_HOST_MEMORY:
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		default:
			AGI_ERROR("vkWaitForFences failed with result: {}", vkGetResultString(result));
			return false;
		}
	}

	void VulkanFence::Reset()
	{
		if (m_IsSignaled)
		{
			VK_CHECK(vkResetFences, m_Context->GetDevice().Logical, 1, &m_RendererID);
			m_IsSignaled = false;
		}
	}

};