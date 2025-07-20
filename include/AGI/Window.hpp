#pragma once

#include "agipch.hpp"

#include "Settings.hpp"

#include <glm/glm.hpp>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace AGI {

	class Window
	{
	public:
		Window(Settings& settings);
		~Window();

		void Init();
		void OnUpdate();

		inline uint32_t GetWidth() const { return m_Settings.Window.Width; }
		inline uint32_t GetHeight() const { return m_Settings.Window.Height; }
		const WindowProps& GetProperties() const { return m_Settings.Window; }
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

		void* GetNativeWindow() const;
		GLFWwindow* GetGlfwWindow() const { return m_Window; }

		// Events
		void SetWindowPosCallback(WindowPosFunc callback) { m_Settings.Window.WindowPosCallback = callback; InstallCallbacks(); }
		void SetWindowSizeCallback(WindowSizeFunc callback) { m_Settings.Window.WindowSizeCallback = callback; InstallCallbacks(); }
		void SetWindowCloseCallback(WindowCloseFunc callback) { m_Settings.Window.WindowCloseCallback = callback; InstallCallbacks(); }
		void SetWindowRefreshCallback(WindowRefreshFunc callback) { m_Settings.Window.WindowRefreshCallback = callback; InstallCallbacks();}
		void SetWindowFocusCallback(WindowFocusFunc callback) { m_Settings.Window.WindowFocusCallback = callback; InstallCallbacks(); }
		void SetWindowIconifyCallback(WindowIconifyFunc callback) { m_Settings.Window.WindowIconifyCallback = callback; InstallCallbacks(); }
		void SetWindowMaximizeCallback(WindowMaximizeFunc callback) { m_Settings.Window.WindowMaximizeCallback = callback; InstallCallbacks(); }
		void SetFramebufferSizeCallback(FramebufferSizeFunc callback) { m_Settings.Window.FramebufferSizeCallback = callback; InstallCallbacks(); }
		void SetContentScaleCallback(ContentScaleFunc callback) { m_Settings.Window.WindowContentScaleCallback = callback; InstallCallbacks(); }
		void SetMouseButtonCallback(MouseButtonFunc callback) { m_Settings.Window.MouseButtonCallback = callback; InstallCallbacks(); }
		void SetCursorPosCallback(CursorPosFunc callback) { m_Settings.Window.CursorPosCallback = callback; InstallCallbacks(); }
		void SetCursorEnterCallback(CursorEnterFunc callback) { m_Settings.Window.CursorEnterCallback = callback; InstallCallbacks(); }
		void SetScrollCallback(ScrollFunc callback) { m_Settings.Window.ScrollCallback = callback; InstallCallbacks(); }
		void SetKeyCallback(KeyFunc callback) { m_Settings.Window.KeyCallback = callback; InstallCallbacks(); }
		void SetCharCallback(CharFunc callback) { m_Settings.Window.CharCallback = callback; InstallCallbacks(); }
		void SetCharModsCallback(CharModsFunc callback) { m_Settings.Window.CharModsCallback = callback; InstallCallbacks(); }
		void SetDropCallback(DropFunc callback) { m_Settings.Window.DropCallback = callback; InstallCallbacks(); }

		static float GetTime() { return glfwGetTime(); }
		static void* LoaderFunc(const char* name) { return (void*)glfwGetProcAddress(name); }
		static std::unique_ptr<Window> Create(Settings& settings) { return std::make_unique<Window>(settings); }
	private:
		void InstallCallbacks();
	private:
		GLFWwindow* m_Window;
		Settings m_Settings;

		friend class RenderContext;
	};

}