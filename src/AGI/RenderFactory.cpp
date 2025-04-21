#include "agipch.h"

#include "AGI/Buffer.h"
#include "AGI/RenderAPI.h"
#include "AGI/Shader.h"
#include "AGI/Texture.h"
#include "AGI/VertexArray.h"
#include "AGI/Framebuffer.h"

#include "OpenGLBuffer.h"
#include "OpenGLRenderAPI.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLVertexArray.h"
#include "OpenGLFramebuffer.h"

namespace AGI {

	std::unique_ptr<RenderAPI> RenderAPI::Create(API api)
	{
		s_CurrentAPI = api;

		switch (api)
		{
			case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case API::OpenGL:  return std::make_unique<OpenGLRenderAPI>();
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (RenderAPI::GetAPI())
		{
		case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(size);
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RenderAPI::GetAPI())
		{
		case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (RenderAPI::GetAPI())
		{
		case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RenderAPI::GetAPI())
		{
		case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case API::OpenGL:  return std::make_shared<OpenGLShader>(filepath);
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RenderAPI::GetAPI())
		{
		case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case API::OpenGL:  return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Texture> Texture::Create(uint32_t width, uint32_t height, uint16_t channels = 4)
	{
		switch (RenderAPI::GetAPI())
		{
		case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case API::OpenGL:  return std::make_shared<OpenGLTexture>(width, height, channels);
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Texture> Texture::Create(const std::string& path)
	{
		switch (RenderAPI::GetAPI())
		{
		case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case API::OpenGL:  return std::make_shared<OpenGLTexture>(path);
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (RenderAPI::GetAPI())
		{
		case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case API::OpenGL:  return std::make_shared<OpenGLVertexArray>();
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Framebuffer> Framebuffer::Create(int width, int height)
	{
		switch (RenderAPI::GetAPI())
		{
		case API::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case API::OpenGL:  return std::make_shared<OpenGLFramebuffer>(width, height);
		}

		AGI_VERIFY(false, "Unknown RendererAPI!");
		return nullptr;
	}

}