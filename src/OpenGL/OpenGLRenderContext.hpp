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
		virtual bool Init() override;
		virtual void Shutdown() override;

		virtual void DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount = 0) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void SetTextureAlignment(int align) override;
		virtual void Clear() override;

		virtual VertexBuffer CreateVertexBuffer(uint32_t vertices, const BufferLayout& layout) override { return std::make_shared<OpenGLVertexBuffer>(vertices, layout); }
		virtual IndexBuffer CreateIndexBuffer(uint32_t* indices, uint32_t size) override { return std::make_shared<OpenGLIndexBuffer>(indices, size); }
		virtual Framebuffer CreateFramebuffer(const FramebufferSpecification& spec) override { return std::make_shared<OpenGLFramebuffer>(spec); }
		virtual Shader CreateShader(const ShaderSources& shaderSources) override { return std::make_shared<OpenGLShader>(shaderSources); }
		virtual Texture CreateTexture(const TextureSpecification& spec) override { return std::make_shared<OpenGLTexture>(spec); }
		virtual VertexArray CreateVertexArray() override { return std::make_shared<OpenGLVertexArray>(); }
	};

}