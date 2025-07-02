#pragma once

#include "AGI/RenderContext.hpp"
#include "VulkanUtils.hpp"

#include <vulkan/vulkan.h>

constexpr const char* SDK_VALIDATION_LAYER = "VK_LAYER_KHRONOS_validation";

namespace AGI {

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
		std::vector<const char*> GetRequiredExtensions();
		std::vector<const char*> GetRequiredLayers();
	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};

}