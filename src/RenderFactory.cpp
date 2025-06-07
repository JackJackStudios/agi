#include "agipch.hpp"

#include "AGI/Buffer.hpp"
#include "AGI/RenderContext.hpp"
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

#define GENERATE_FACTORY(x, ...) switch (RenderContext::GetCurrentContext()->GetType()) \
                                 { \
									case APIType::Headless: AGI_VERIFY(false, #x "::Headless is currently not supported!"); return nullptr;  \
									case APIType::OpenGL:  return std::make_shared<OpenGL##x>(__VA_ARGS__);  \
                                 } \
                                 AGI_VERIFY(false, "Unknown " #x); \
                                 return nullptr;

namespace AGI {

	APIType BestAPI()
	{
		return APIType::OpenGL;
	}

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

	void RenderContext::Shutdown(std::unique_ptr<RenderContext>& api)
	{
		api.reset();
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