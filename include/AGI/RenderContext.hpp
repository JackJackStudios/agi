#pragma once

#include "VertexArray.hpp"
#include "Log.hpp"

#include <glm/glm.hpp>

namespace msdfgen {

	struct FreetypeHandle;

};

namespace AGI {

	enum class APIType
	{
		Headless = 0,
		Guess,
		OpenGL,
	};

	typedef void* (* OpenGLloaderFn)(const char *name);

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;
		bool Resize;
		bool Visible;
		bool Decorated;
		bool Maximise;

		WindowProps(const std::string& title = "AGI-Window",
			unsigned int width = 1280,
			unsigned int height = 720,
			bool vsync = true,
			bool resize = true,
			bool visible = true,
			bool decorated = true,
			bool maximise = false)
			: Title(title), Width(width), Height(height), VSync(vsync), 
			  Resize(resize), Visible(visible), Decorated(decorated), 
			  Maximise(maximise)
		{
		}
	};

	struct Settings
	{
		APIType PreferedAPI;
		bool Blending;
		MessageCallbackFn MessageFunc;
		WindowProps WindowProps;
	};

	class RenderContext
	{
	public:
		virtual ~RenderContext() = default;
		
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void SetTextureAlignment(int align) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		
		APIType GetType() const { return m_APIType; }

		static std::unique_ptr<RenderContext> Init(Settings settings);
		static void Shutdown(std::unique_ptr<RenderContext>& api);
		static RenderContext* GetCurrentContext() { return s_CurrentContext; }
	private:
		APIType m_APIType;
		Settings m_Settings;
		msdfgen::FreetypeHandle* m_Freetype;

		inline static RenderContext* s_CurrentContext;

		friend class Font;
	};
}