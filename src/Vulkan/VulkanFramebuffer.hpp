#pragma once
#include "Vulkan.hpp"

#include "VulkanRenderPass.hpp"

namespace AGI {

	class VulkanContext;

	class VulkanFramebuffer
	{
	public:
		VulkanFramebuffer() = default;

		bool Create(VulkanContext* context, VulkanRenderPass* renderpass, const glm::vec2& size, const std::vector<VkImageView>& attachments);
		void Destroy();

		VkFramebuffer GetHandle() const { return m_RendererID; }
	private:
		VkFramebuffer m_RendererID = nullptr;

		VulkanContext* m_BoundContext = nullptr;
		VulkanRenderPass* m_Parent = nullptr;

		std::vector<VkImageView> m_Attachments;
	};

};