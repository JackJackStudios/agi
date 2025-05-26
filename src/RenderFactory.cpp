#include "agipch.h"

#include "AGI/Buffer.h"
#include "AGI/RenderAPI.h"
#include "AGI/Shader.h"
#include "AGI/Texture.h"
#include "AGI/VertexArray.h"
#include "AGI/Framebuffer.h"

#include "OpenGL/OpenGLBuffer.h"
#include "OpenGL/OpenGLRenderAPI.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLTexture.h"
#include "OpenGL/OpenGLVertexArray.h"
#include "OpenGL/OpenGLFramebuffer.h"

namespace AGI {

	APIType BestAPI()
	{
		return APIType::OpenGL;
	}

	std::unique_ptr<RenderAPI> RenderAPI::Create(RenderAPISetttings settings)
	{
		std::unique_ptr<RenderAPI> newapi;
		APIType newtype = settings.PreferedAPI == APIType::Guess ? BestAPI() : settings.PreferedAPI;

		switch (newtype)
		{
			case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case APIType::OpenGL:  newapi = std::make_unique<OpenGLRenderAPI>(settings);
		}

		s_CurrentAPI = newapi.get();

		newapi->m_APIType = newtype;
		return std::move(newapi);
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(size);
		}

		AGI_VERIFY(false, "Unknown VertexBuffer!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		AGI_VERIFY(false, "Unknown VertexBuffer!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t vertices, const BufferLayout& layout)
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, layout);
		}

		AGI_VERIFY(false, "Unknown VertexBuffer!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		AGI_VERIFY(false, "Unknown IndexBuffer!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLShader>(filepath);
		}

		AGI_VERIFY(false, "Unknown Shader!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		AGI_VERIFY(false, "Unknown Shader!");
		return nullptr;
	}

	std::shared_ptr<Texture> Texture::Create(TextureSpecifaction spec)
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLTexture>(spec);
		}

		AGI_VERIFY(false, "Unknown Texture!");
		return nullptr;
	}

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		AGI_VERIFY(false, "Unknown VertexArray!");
		return nullptr;
	}

	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLFramebuffer>(spec);
		}

		AGI_VERIFY(false, "Unknown Framebuffer!");
		return nullptr;
	}

}