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

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount = 0) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColour(const glm::vec4& colour) override;

		virtual VertexBuffer CreateVertexBuffer(uint32_t vertices, const BufferLayout& layout) override { return ResourceBarrier<OpenGLVertexBuffer>::Create(vertices, layout); }
		virtual IndexBuffer CreateIndexBuffer(uint32_t* indices, uint32_t size) override                { return ResourceBarrier<OpenGLIndexBuffer>::Create(indices, size); }
		virtual Framebuffer CreateFramebuffer(const FramebufferSpecification& spec) override            { return ResourceBarrier<OpenGLFramebuffer>::Create(spec); }
		virtual Shader CreateShader(const ShaderSources& shaderSources) override                        { return ResourceBarrier<OpenGLShader>::Create(shaderSources); }
		virtual Texture CreateTexture(const TextureSpecification& spec) override                        { return ResourceBarrier<OpenGLTexture>::Create(spec); }
		virtual VertexArray CreateVertexArray() override                                                { return ResourceBarrier<OpenGLVertexArray>::Create(); }
	};

}