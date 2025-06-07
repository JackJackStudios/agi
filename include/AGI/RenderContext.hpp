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
	typedef void(* 	WindowPosFunc) (glm::vec2 pos);
	typedef void(* 	WindowSizeFunc) (glm::vec2 size);
	typedef void(* 	WindowCloseFunc) ();
	typedef void(* 	WindowRefreshFunc) ();
	typedef void(* 	WindowFocusFunc) (bool focused);
	typedef void(* 	WindowIconifyFunc) (bool iconified);
	typedef void(* 	WindowMaximizeFunc) (bool maximized);
	typedef void(* 	FramebufferSizeFunc) (glm::vec2 size);
	typedef void(* 	ContentScaleFunc) (glm::vec2 scale);
	typedef void(* 	MouseButtonFunc) (int button, int action, int mods);
	typedef void(* 	CursorPosFunc) (glm::vec2 pos);
	typedef void(* 	CursorEnterFunc) (bool entered);
	typedef void(* 	ScrollFunc) (glm::vec2 offset);
	typedef void(* 	KeyFunc) (int key, int scancode, int action, int mods);
	typedef void(* 	CharFunc) (unsigned int codepoint);
	typedef void(* 	CharModsFunc) (unsigned int codepoint, int mods);
	typedef void(* 	DropFunc) (int path_count, const char *paths[]);

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
		
		WindowPosFunc WindowPosCallback = nullptr;
		WindowSizeFunc WindowSizeCallback = nullptr;
		WindowCloseFunc WindowCloseCallback = nullptr;
		WindowRefreshFunc WindowRefreshCallback = nullptr;
		WindowFocusFunc WindowFocusCallback = nullptr;
		WindowIconifyFunc WindowIconifyCallback = nullptr;
		WindowMaximizeFunc WindowMaximizeCallback = nullptr;
		FramebufferSizeFunc FramebufferSizeCallback = nullptr;
		ContentScaleFunc WindowContentScaleCallback = nullptr;
		MouseButtonFunc MouseButtonCallback = nullptr;
		CursorPosFunc CursorPosCallback = nullptr;
		CursorEnterFunc CursorEnterCallback = nullptr;
		ScrollFunc ScrollCallback = nullptr;
		KeyFunc KeyCallback = nullptr;
		CharFunc CharCallback = nullptr;
		CharModsFunc CharModsCallback = nullptr;
		DropFunc DropCallback = nullptr;
		
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