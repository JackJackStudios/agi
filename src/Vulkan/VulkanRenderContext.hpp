#pragma once

#include "AGI/RenderContext.hpp"
#include "Vulkan.hpp"

#include "VulkanRenderPass.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanFramebuffer.hpp"

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

	struct VulkanDevice
	{
		VkPhysicalDevice Physical = nullptr;
		VkDevice Logical;

		QueueIndexes QueueInfo;
		SwapchainSupport SwapchainInfo;

		VkQueue GraphicsQueue;
		VkQueue PresentQueue;
		VkQueue TransferQueue;

		VkCommandPool GraphicsPool;
	};

	struct VulkanSwapchain
	{
		VkSwapchainKHR Handle;
		std::vector<VkImage> Images;
		std::vector<VkImageView> ImageViews;
		std::vector<VulkanFramebuffer> Framebuffers;

		uint8_t FramesInFlight = 3;
		VkSurfaceFormatKHR ImageFormat;
	};


	class VulkanContext : public RenderContext
	{
	public:
		virtual bool Init() override;
		virtual void Shutdown() override;
		
		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount = 0) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColour(const glm::vec4& colour) override;

		virtual VertexBuffer CreateVertexBuffer(uint32_t vertices, const BufferLayout& layout) override { return nullptr; }
		virtual IndexBuffer CreateIndexBuffer(uint32_t* indices, uint32_t size) override { return nullptr; }
		virtual Framebuffer CreateFramebuffer(const FramebufferSpecification& spec) override { return nullptr; }
		virtual Shader CreateShader(const ShaderSources& shaderSources) override { return nullptr; }
		virtual Texture CreateTexture(const TextureSpecification& spec) override { return nullptr; }
		virtual VertexArray CreateVertexArray() override { return nullptr; }

		const VulkanDevice& GetDevice() const { return m_Device; }
		const VulkanSwapchain& GetSwapchain() const { return m_Swapchain; }
		const VkAllocationCallbacks* GetAllocator() const { return m_Allocator; }

	private:
		bool MatchPhysicalDevice(VkPhysicalDevice* chosen_device, DeviceRequirements& requirements);
		void QuerySwapchainSupport(VkPhysicalDevice device, SwapchainSupport* swapchain_info);
		bool DeviceMeetsRequirements(VkPhysicalDevice device, QueueIndexes* queue_info, SwapchainSupport* swapchain_support, const DeviceRequirements* requirements);

		bool CreateDevice();
		void DestroyDevice();

		bool CreateSwapchain(const glm::uvec2& size);
		void DestroySwapchain();

		bool AcquireNextImage(uint64_t timeout, VkSemaphore signal_semaphore, VkFence fence, uint32_t* out_image);
		bool PresentSwapchain(VkSemaphore wait_semaphore, uint32_t image);
	private:
		VkDebugUtilsMessengerEXT m_Debugger;

		VkInstance m_Instance;
		VkAllocationCallbacks* m_Allocator = nullptr;

		VkSurfaceKHR m_WindowSurface;

		VulkanDevice m_Device;
		VulkanSwapchain m_Swapchain;
		VulkanRenderPass m_MainRenderpass;

		// TODO: What are these for? :/
		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_QueueCompleteSemaphores;
		std::vector<VkFence> m_InFlightFences;

		std::vector<VkFence> m_ImagesInFlight;

		std::vector<VulkanCommandBuffer> m_GraphicsCommands;
		glm::vec4 m_ClearColour = { 1.0f, 1.0f, 1.0f, 1.0f };

		uint32_t m_CurrentFrame = 0;
		uint32_t m_ImageIndex = 0;
	};

}
