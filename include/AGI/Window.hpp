#pragma once

#include "agipch.hpp"
#include "Settings.hpp"

#include <glm/glm.hpp>

struct GLFWwindow;

namespace AGI {

	class Window
	{
	public:
		Window(const Settings& settings, const WindowProps& props);

		void Init();
		void Shutdown();

		// Commands
		void SetTitle(const std::string& title);
		void SetVisable(bool enabled);
		bool ShouldClose(bool closing = false);
		void PollEvents();
		void CenterScreen();
		float GetDelta();

		// Window attributes
		glm::uvec2 GetSize() const { return m_Properties.Size; }
		std::string GetTitle() const { return m_Properties.Title; }
		glm::vec2 GetPosition() const;

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
		void SetScrollCallback(ScrollFunc callback)                 { m_Events.ScrollCallback = callback;         InstallCallbacks(); }
		void SetKeyCallback(KeyFunc callback)                       { m_Events.KeyCallback = callback;            InstallCallbacks(); }
		void SetCharCallback(CharFunc callback)                     { m_Events.CharCallback = callback;           InstallCallbacks(); }
		void SetDropCallback(DropFunc callback)                     { m_Events.DropCallback = callback;           InstallCallbacks(); }

		static Window* Create(Settings& settings, WindowProps& props) { return new Window(settings, props); }
	private:
		void InstallCallbacks();
	private:
		Settings m_Settings;
		WindowProps m_Properties;

		struct
		{
			WindowPosFunc WindowPosCallback = nullptr;
			WindowSizeFunc WindowSizeCallback = nullptr;
			WindowCloseFunc WindowCloseCallback = nullptr;
			WindowFocusFunc WindowFocusCallback = nullptr;
			WindowMaximizeFunc WindowMaximizeCallback = nullptr;
			MouseButtonFunc MouseButtonCallback = nullptr;
			CursorPosFunc CursorPosCallback = nullptr;
			ScrollFunc ScrollCallback = nullptr;
			KeyFunc KeyCallback = nullptr;
			CharFunc CharCallback = nullptr;
			DropFunc DropCallback = nullptr;
		} m_Events;

		GLFWwindow* m_Window = nullptr;

		int m_WindowIndex = -1;
		float m_LastFrameTime = 0.0f;

		friend class RenderContext;
	};

}