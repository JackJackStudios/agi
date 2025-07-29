#pragma once

#include "AGI/RenderContext.hpp"
#include "Vulkan.hpp"

namespace AGI {

	struct DeviceRequirements
	{
		bool Graphics;
		bool Compute;
		bool Present;
		bool Transfer;

		VkPhysicalDeviceType DeviceType;
		std::vector<const char*> Extensions;
	};

	struct QueueIndexes
	{
		// -1 = not supported.
		uint32_t GraphicsIndex = -1;
		uint32_t PresentIndex = -1;
		uint32_t ComputeIndex = -1;
		uint32_t TransferIndex = -1;
	};

	struct SwapchainSupport
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	class VulkanContext : public RenderContext
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount = 0) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void SetTextureAlignment(int align) override;
		virtual void Clear() override;

		virtual VertexBuffer CreateVertexBuffer(uint32_t vertices, const BufferLayout& layout) override { return nullptr; }
		virtual IndexBuffer CreateIndexBuffer(uint32_t* indices, uint32_t size) override { return nullptr; }
		virtual Framebuffer CreateFramebuffer(const FramebufferSpecification& spec) override { return nullptr; }
		virtual Shader CreateShader(const ShaderSources& shaderSources) override { return nullptr; }
		virtual Texture CreateTexture(const TextureSpecification& spec) override { return nullptr; }
		virtual VertexArray CreateVertexArray() override { return nullptr; }
	private:
		bool MatchPhysicalDevice(VkPhysicalDevice* chosen_device, DeviceRequirements& requirements);
		void QuerySwapchainSupport(VkPhysicalDevice device, SwapchainSupport* swapchain_info);
		bool DeviceMeetsRequirements(
			VkPhysicalDevice device,
			QueueIndexes* queue_info,
			SwapchainSupport* swapchain_support,
			const DeviceRequirements* requirements);

		bool CreateDevice();
		void DestroyDevice();
	private:
#ifdef AGI_DEBUG
		VkDebugUtilsMessengerEXT m_Debugger;
#endif
		VkInstance m_Instance;
		VkAllocationCallbacks* m_Allocator = nullptr;

		VkSurfaceKHR m_WindowSurface;

		struct VulkanDevice
		{
			VkPhysicalDevice Physical = nullptr;
			VkDevice Logical;

			QueueIndexes QueueInfo;
			SwapchainSupport SwapchainInfo;

			VkQueue GraphicsQueue;
			VkQueue PresentQueue;
			VkQueue TransferQueue;
		} m_Device;
	};

}