#pragma once

#include "RenderContext.hpp"
#include "Log.hpp"

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

	class Window
	{
	public:
		Window(const std::unique_ptr<RenderContext>& context, bool initContext);
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
		static std::unique_ptr<Window> Create(const std::unique_ptr<RenderContext>& context, bool initContext = false) { return std::make_unique<Window>(context, initContext); }
	private:
		void InstallCallbacks();
	private:
		GLFWwindow* m_Window;
		WindowProps m_Data;
	};

}