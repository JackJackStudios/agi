#pragma once

#include "VertexArray.hpp"
#include "Log.hpp"

#include <glm/glm.hpp>

namespace AGI {

	enum class APIType
	{
		Headless = 0,
		Guess,
		OpenGL,
		Vulkan, // Coming soon :)
	};

	typedef void* (* OpenGLloaderFn)(const char *name);

	struct Settings
	{
		APIType PreferedAPI;
		bool Blending;
		MessageCallbackFn MessageFunc;
	};

	class RenderContext
	{
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void SetTextureAlignment(int align) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		
		APIType GetType() const { return m_Settings.PreferedAPI; }

		static std::unique_ptr<RenderContext> Create(Settings settings);
		static void Shutdown(std::unique_ptr<RenderContext>& api);
		static RenderContext* GetCurrentContext() { return s_CurrentContext; }
	protected:
		Settings m_Settings;
	private:
		inline static RenderContext* s_CurrentContext;

		friend class Font;
		friend class Window;
	};
}