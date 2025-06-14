#pragma once

#include "agipch.hpp"

#include "AGI/RenderContext.hpp"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace AGI {

#if defined(AGI_WINDOWS)
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
    using NativeWindow = HWND;
 
#elif defined(AGI_MACOSX)
    #define GLFW_EXPOSE_NATIVE_COCOA
    #include <GLFW/glfw3native.h>
    using NativeWindow = void*; // NSWindow*

#elif defined(AGI_LINUX)
    #define GLFW_EXPOSE_NATIVE_X11
    #define GLFW_EXPOSE_NATIVE_WAYLAND
    #include <GLFW/glfw3native.h>
    using NativeWindow = void*; // Will be X11 Window or wl_surface*

#endif

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

	class Window
	{
	public:
		Window(const WindowProps& windowProps, const std::unique_ptr<RenderContext>& context);
		~Window();

		void OnUpdate();

		inline uint32_t GetWidth() const { return m_Data.Width; }
		inline uint32_t GetHeight() const { return m_Data.Height; }
		glm::vec2 GetPosition() const;

		// Window attributes
		bool ShouldClose() const;
		bool IsVSync() const;

		void SetVSync(bool enabled);
		void SetVisable(bool enabled);
		void SetTitle(const std::string& title);

		bool IsKeyOn(int32_t key) const;
		bool IsMouseButtonOn(int32_t button) const;
		glm::vec2 GetCursorPos() const;

		NativeWindow GetNativeWindow() const;
		GLFWwindow* GetGlfwWindow() const { return m_Window; }

		// Events
		void SetWindowPosCallback(WindowPosFunc callback) { m_Data.WindowPosCallback = callback; InstallCallbacks(); }
		void SetWindowSizeCallback(WindowSizeFunc callback) { m_Data.WindowSizeCallback = callback; InstallCallbacks(); }
		void SetWindowCloseCallback(WindowCloseFunc callback) { m_Data.WindowCloseCallback = callback; InstallCallbacks(); }
		void SetWindowRefreshCallback(WindowRefreshFunc callback) { m_Data.WindowRefreshCallback = callback; InstallCallbacks();}
		void SetWindowFocusCallback(WindowFocusFunc callback) { m_Data.WindowFocusCallback = callback; InstallCallbacks(); }
		void SetWindowIconifyCallback(WindowIconifyFunc callback) { m_Data.WindowIconifyCallback = callback; InstallCallbacks(); }
		void SetWindowMaximizeCallback(WindowMaximizeFunc callback) { m_Data.WindowMaximizeCallback = callback; InstallCallbacks(); }
		void SetFramebufferSizeCallback(FramebufferSizeFunc callback) { m_Data.FramebufferSizeCallback = callback; InstallCallbacks(); }
		void SetContentScaleCallback(ContentScaleFunc callback) { m_Data.WindowContentScaleCallback = callback; InstallCallbacks(); }
		void SetMouseButtonCallback(MouseButtonFunc callback) { m_Data.MouseButtonCallback = callback; InstallCallbacks(); }
		void SetCursorPosCallback(CursorPosFunc callback) { m_Data.CursorPosCallback = callback; InstallCallbacks(); }
		void SetCursorEnterCallback(CursorEnterFunc callback) { m_Data.CursorEnterCallback = callback; InstallCallbacks(); }
		void SetScrollCallback(ScrollFunc callback) { m_Data.ScrollCallback = callback; InstallCallbacks(); }
		void SetKeyCallback(KeyFunc callback) { m_Data.KeyCallback = callback; InstallCallbacks(); }
		void SetCharCallback(CharFunc callback) { m_Data.CharCallback = callback; InstallCallbacks(); }
		void SetCharModsCallback(CharModsFunc callback) { m_Data.CharModsCallback = callback; InstallCallbacks(); }
		void SetDropCallback(DropFunc callback) { m_Data.DropCallback = callback; InstallCallbacks(); }

		static float GetTime() { return glfwGetTime(); }
		static void* LoaderFunc(const char* name) { return (void*)glfwGetProcAddress(name); }
		static std::unique_ptr<Window> Create(const WindowProps& windowProps, const std::unique_ptr<RenderContext>& context) { return std::make_unique<Window>(windowProps, context); }
	private:
		void InstallCallbacks();
	private:
		GLFWwindow* m_Window;
		WindowProps m_Data;
	};

}