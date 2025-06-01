#include "agipch.hpp"

#include "AGI/Buffer.hpp"
#include "AGI/RenderAPI.hpp"
#include "AGI/Shader.hpp"
#include "AGI/Texture.hpp"
#include "AGI/VertexArray.hpp"
#include "AGI/Framebuffer.hpp"

#include "OpenGL/OpenGLBuffer.hpp"
#include "OpenGL/OpenGLRenderAPI.hpp"
#include "OpenGL/OpenGLShader.hpp"
#include "OpenGL/OpenGLTexture.hpp"
#include "OpenGL/OpenGLVertexArray.hpp"
#include "OpenGL/OpenGLFramebuffer.hpp"

#undef INFINITE
#include "msdf-atlas-gen/msdf-atlas-gen.h"

namespace AGI {

	APIType BestAPI()
	{
		return APIType::OpenGL;
	}

	ImageFormat ChannelsToImageFormat(uint16_t channels)
	{
		switch (channels)
		{
		case 0: return ImageFormat::None;
		case 1: return ImageFormat::RED;
		case 2: return ImageFormat::RG;
		case 3: return ImageFormat::RGB;
		case 4: return ImageFormat::RGBA;
		}

		AGI_WARN("Unknown channels count");
		return ImageFormat::None;
	}

	uint16_t ImageFormatToChannels(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::None: return 0;
		case ImageFormat::RED:  return 1;
		case ImageFormat::RG:   return 2;
		case ImageFormat::RGB:  return 3;
		case ImageFormat::RGBA: return 4;
		}

		AGI_WARN("Unknown ImageFormat");
		return 0;
	}

	ShaderType StringToShaderType(const std::string& type)
	{
		if (type == "vertex")   return ShaderType::Vertex;
		if (type == "fragment") return ShaderType::Fragment;
		if (type == "pixel")    return ShaderType::Fragment;

		AGI_VERIFY(false, "Unknown shader type '{}'", type);
		return ShaderType::None;
	}

	ShaderSources Shader::ProcessSource(const std::string& source)
	{
		ShaderSources shaderSources;

		const std::string typeToken = "#type";
		size_t typeTokenLength = typeToken.length();
		size_t pos = 0;

		while ((pos = source.find(typeToken, pos)) != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			AGI_VERIFY(eol != std::string::npos, "Syntax error: Missing end of line after #type");

			size_t typeStart = pos + typeTokenLength + 1;
			std::string typeStr = source.substr(typeStart, eol - typeStart);
			ShaderType shaderType = StringToShaderType(typeStr);

			size_t codeStart = source.find_first_not_of("\r\n", eol);
			AGI_VERIFY(codeStart != std::string::npos, "Syntax error: No shader code after #type");

			size_t nextTypePos = source.find(typeToken, codeStart);

			std::string shaderCode = (nextTypePos == std::string::npos)
				? source.substr(codeStart)
				: source.substr(codeStart, nextTypePos - codeStart);

			shaderSources[shaderType] = shaderCode;
			pos = nextTypePos;
		}

		return shaderSources;
	}

	std::unique_ptr<RenderAPI> RenderAPI::Init(RenderAPISetttings settings)
	{
		std::unique_ptr<RenderAPI> newapi;
		APIType newtype = settings.PreferedAPI == APIType::Guess ? BestAPI() : settings.PreferedAPI;

		Log::Init(settings.MessageFunc);

		switch (newtype)
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  newapi = std::make_unique<OpenGLRenderAPI>(settings);
		}

		s_CurrentAPI = newapi.get();

		newapi->m_APIType = newtype;
		newapi->m_Freetype = msdfgen::initializeFreetype();
		return std::move(newapi);
	}

	void RenderAPI::Shutdown(std::unique_ptr<RenderAPI>& api)
	{
		msdfgen::deinitializeFreetype(api->m_Freetype);

		api.reset();
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

	std::shared_ptr<Shader> Shader::Create(const ShaderSources& shaderSources)
	{
		switch (RenderAPI::GetCurrentAPI()->GetType())
		{
		case APIType::None:    AGI_VERIFY(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case APIType::OpenGL:  return std::make_shared<OpenGLShader>(shaderSources);
		}

		AGI_VERIFY(false, "Unknown Shader!");
		return nullptr;
	}

	std::shared_ptr<Texture> Texture::Create(TextureSpecification spec)
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