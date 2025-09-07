#pragma once
#include "Vulkan.hpp"

namespace AGI {

	class VulkanContext;

	class VulkanFence
	{
	public:
		bool Create(VulkanContext* context, bool create_signaled);
		void Destroy();

		bool Wait(uint64_t timeout);
		void Reset();

		VkFence GetHandle() const { return m_RendererID; }
	private:
		VkFence m_RendererID = nullptr;
		bool m_IsSignaled = false;

		VulkanContext* m_Context = nullptr;
	};

};