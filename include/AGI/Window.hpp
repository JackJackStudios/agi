#pragma once

#include "agipch.hpp"

#include "Settings.hpp"

#include <glm/glm.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace AGI {

	class Window
	{
	public:
		Window(Settings& settings);

		void Init();
		void OnUpdate();
		void Shutdown();

		inline uint32_t GetWidth() const { return m_Settings.Window.Width; }
		inline uint32_t GetHeight() const { return m_Settings.Window.Height; }
		const WindowProps& GetProperties() const { return m_Settings.Window; }
		glm::vec2 GetPosition() const;

		// Window attributes
		bool ShouldClose(bool closing = false) const;
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
		void SetWindowPosCallback(WindowPosFunc callback)           { m_Events.WindowPosCallback = callback;      InstallCallbacks(); }
		void SetWindowSizeCallback(WindowSizeFunc callback)         { m_Events.WindowSizeCallback = callback;     InstallCallbacks(); }
		void SetWindowCloseCallback(WindowCloseFunc callback)       { m_Events.WindowCloseCallback = callback;    InstallCallbacks(); }
		void SetWindowFocusCallback(WindowFocusFunc callback)       { m_Events.WindowFocusCallback = callback;    InstallCallbacks(); }
		void SetWindowMaximizeCallback(WindowMaximizeFunc callback) { m_Events.WindowMaximizeCallback = callback; InstallCallbacks(); }
		void SetMouseButtonCallback(MouseButtonFunc callback)       { m_Events.MouseButtonCallback = callback;    InstallCallbacks(); }
		void SetCursorPosCallback(CursorPosFunc callback)           { m_Events.CursorPosCallback = callback;      InstallCallbacks(); }
		void SetCursorEnterCallback(CursorEnterFunc callback)       { m_Events.CursorEnterCallback = callback;    InstallCallbacks(); }
		void SetScrollCallback(ScrollFunc callback)                 { m_Events.ScrollCallback = callback;         InstallCallbacks(); }
		void SetKeyCallback(KeyFunc callback)                       { m_Events.KeyCallback = callback;            InstallCallbacks(); }
		void SetCharCallback(CharFunc callback)                     { m_Events.CharCallback = callback;           InstallCallbacks(); }
		void SetDropCallback(DropFunc callback)                     { m_Events.DropCallback = callback;           InstallCallbacks(); }

		static float GetTime() { return glfwGetTime(); }
		static void* LoaderFunc(const char* name) { return (void*)glfwGetProcAddress(name); }
		static Window* Create(Settings& settings) { return new Window(settings); }
	private:
		void InstallCallbacks();
	private:
		GLFWwindow* m_Window;
		Settings m_Settings;

		struct
		{
			WindowPosFunc WindowPosCallback = nullptr;
			WindowSizeFunc WindowSizeCallback = nullptr;
			WindowCloseFunc WindowCloseCallback = nullptr;
			WindowFocusFunc WindowFocusCallback = nullptr;
			WindowMaximizeFunc WindowMaximizeCallback = nullptr;
			MouseButtonFunc MouseButtonCallback = nullptr;
			CursorPosFunc CursorPosCallback = nullptr;
			CursorEnterFunc CursorEnterCallback = nullptr;
			ScrollFunc ScrollCallback = nullptr;
			KeyFunc KeyCallback = nullptr;
			CharFunc CharCallback = nullptr;
			DropFunc DropCallback = nullptr;
		} m_Events;

		int m_WindowIndex = -1;
		friend class RenderContext;
	};

}