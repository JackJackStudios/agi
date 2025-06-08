#include "agipch.hpp"
#include <AGI/agi.hpp>

#include "OpenGL/OpenGLBuffer.hpp"
#include "OpenGL/OpenGLRenderAPI.hpp"
#include "OpenGL/OpenGLShader.hpp"
#include "OpenGL/OpenGLTexture.hpp"
#include "OpenGL/OpenGLVertexArray.hpp"
#include "OpenGL/OpenGLFramebuffer.hpp"

#define GENERATE_FACTORY(x, ...) switch (RenderContext::GetCurrentContext()->GetType()) \
                                 { \
									case APIType::Headless: AGI_VERIFY(false, #x "::Headless is currently not supported!"); return nullptr;  \
									case APIType::OpenGL:  return std::make_shared<OpenGL##x>(__VA_ARGS__);  \
                                 } \
                                 AGI_VERIFY(false, "Unknown " #x); \
                                 return nullptr;

namespace AGI {


	std::unique_ptr<RenderContext> RenderContext::Create(Settings settings)
	{
		std::unique_ptr<RenderContext> newapi;
		settings.PreferedAPI = settings.PreferedAPI == APIType::Guess ? BestAPI() : settings.PreferedAPI;

		Log::Init(settings.MessageFunc);

		switch (settings.PreferedAPI)
		{
			case APIType::Headless: AGI_VERIFY(false, "RendererAPI::Headless is currently not supported!"); return nullptr;
			case APIType::OpenGL:   newapi = std::make_unique<OpenGLContext>();
		}

		s_CurrentContext = newapi.get();

		newapi->m_Settings = settings;
		return std::move(newapi);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		GENERATE_FACTORY(VertexBuffer, size)
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		GENERATE_FACTORY(VertexBuffer, vertices, size);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t vertices, const BufferLayout& layout)
	{
		GENERATE_FACTORY(VertexBuffer, vertices, layout);
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		GENERATE_FACTORY(IndexBuffer, indices, size);
	}

	std::shared_ptr<Shader> Shader::Create(const ShaderSources& shaderSources)
	{
		GENERATE_FACTORY(Shader, shaderSources);
	}

	std::shared_ptr<Texture> Texture::Create(TextureSpecification spec)
	{
		GENERATE_FACTORY(Texture, spec);
	}

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		GENERATE_FACTORY(VertexArray);
	}

	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		GENERATE_FACTORY(Framebuffer, spec);
	}

}