#pragma once

#include "AGI/RenderContext.hpp"

#include "OpenGLBuffer.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLTexture.hpp"
#include "OpenGLVertexArray.hpp"
#include "OpenGLFramebuffer.hpp"

namespace AGI {

	class OpenGLContext : public RenderContext
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void SetTextureAlignment(int align) override;
		virtual void Clear() override;

		virtual std::shared_ptr<VertexBuffer> CreateVertexBuffer(uint32_t vertices, const BufferLayout& layout) override { return std::make_shared<OpenGLVertexBuffer>(vertices, layout); }
		virtual std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t size) override { return std::make_shared<OpenGLIndexBuffer>(indices, size); }
		virtual std::shared_ptr<Framebuffer> CreateFramebuffer(const FramebufferSpecification& spec) override { return std::make_shared<OpenGLFramebuffer>(spec); }
		virtual std::shared_ptr<Shader> CreateShader(const ShaderSources& shaderSources) override { return std::make_shared<OpenGLShader>(shaderSources); }
		virtual std::shared_ptr<Texture> CreateTexture(const TextureSpecification& spec) override { return std::make_shared<OpenGLTexture>(spec); }
		virtual std::shared_ptr<VertexArray> CreateVertexArray() override { return std::make_shared<OpenGLVertexArray>(); }
	};


}