#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "Log.hpp"

#include <glm/glm.hpp>

namespace AGI {

	enum class APIType
	{
		Headless = 0,
		Guess,
		OpenGL,
		Vulkan, // Coming soon :)
	};

	typedef void* (* LoaderFn)(const char *name);

	struct Settings
	{
		APIType PreferedAPI;
		MessageCallbackFn MessageFunc;
		LoaderFn LoaderFunc = nullptr;
		bool Blending;
	};

	APIType BestAPI();

	class RenderContext
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		
		// Global commands
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetTextureAlignment(int align) = 0;
		virtual void Clear() = 0;

		// Creation functions
		virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint32_t vertices, const BufferLayout& layout) = 0;
		virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t size) = 0;
		virtual std::shared_ptr<Framebuffer> CreateFramebuffer(const FramebufferSpecification& spec) = 0;
		virtual std::shared_ptr<Shader> CreateShader(const ShaderSources& shaderSources) = 0;
		virtual std::shared_ptr<Texture> CreateTexture(const TextureSpecification& spec) = 0;
		virtual std::shared_ptr<VertexArray> CreateVertexArray() = 0;
		
		APIType GetType() const { return m_Settings.PreferedAPI; }

		static std::unique_ptr<RenderContext> Create(Settings settings);
		static RenderContext* GetCurrentContext() { return s_CurrentContext; }
	protected:
		Settings m_Settings;
	private:
		inline static RenderContext* s_CurrentContext;

		friend class Font;
		friend class Window;
	};
}