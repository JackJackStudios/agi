#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "Log.hpp"

#include "Settings.hpp"
#include "Window.hpp"

#include <glm/glm.hpp>

namespace AGI {

	class RenderContext
	{
	public:
		virtual ~RenderContext();

		virtual bool Init() = 0;
		virtual void Shutdown() = 0;
		
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		// Global commands
		virtual void DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		// Creation functions
		virtual VertexBuffer CreateVertexBuffer(uint32_t vertices, const BufferLayout& layout) = 0;
		virtual IndexBuffer CreateIndexBuffer(uint32_t* indices, uint32_t size) = 0;
		virtual Framebuffer CreateFramebuffer(const FramebufferSpecification& spec) = 0;
		virtual Shader CreateShader(const ShaderSources& shaderSources) = 0;
		virtual Texture CreateTexture(const TextureSpecification& spec) = 0;
		virtual VertexArray CreateVertexArray() = 0;
		
		APIType GetType() const { return m_Settings.PreferedAPI; }
		Window* GetBoundWindow() const { return m_BoundWindow; }
		const ContextProperties& GetProps() const { return m_Properties; }

		void PrintProperties()
		{
			const char* apiType = "";
			switch (GetType())
			{
			case APIType::OpenGL: apiType = "OpenGL"; break;
			case APIType::Vulkan: apiType = "Vulkan"; break;
			}

			AGI_INFO("Using {} {} - {}", apiType, m_Properties.Version, m_Properties.Renderer);
		}

		static RenderContext* Create(Window* window);
	protected:
		Window* m_BoundWindow;
		Settings m_Settings;
		ContextProperties m_Properties;
	private:
		using ContextFactoryFn = std::function<RenderContext* ()>;
		static inline std::array<ContextFactoryFn, static_cast<size_t>(APIType::__COUNT)> s_ContextFactory = {};

		template<typename T>
		static void RegisterFactory(APIType type)
		{
			s_ContextFactory[static_cast<uint32_t>(type)] = []() -> RenderContext* { return new T(); };
		}

		template<typename T, APIType TApi>
		friend struct Register;
	};

	template<typename T, APIType TApi>
	struct Register
	{
		Register()
		{
			RenderContext::RegisterFactory<T>(TApi);
		}
	};
}