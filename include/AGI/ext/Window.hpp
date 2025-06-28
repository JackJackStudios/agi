#pragma once

#include "agipch.hpp"

#include "AGI/RenderContext.hpp"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace AGI {

	class Window;
	using WindowPosFunc        = std::function<void(Window* window, glm::vec2 pos)>;
using WindowSizeFunc       = std::function<void(Window* window, glm::vec2 size)>;
using WindowCloseFunc      = std::function<void(Window* window)>;
using WindowRefreshFunc    = std::function<void(Window* window)>;
using WindowFocusFunc      = std::function<void(Window* window, bool focused)>;
using WindowIconifyFunc    = std::function<void(Window* window, bool iconified)>;
using WindowMaximizeFunc   = std::function<void(Window* window, bool maximized)>;
using FramebufferSizeFunc  = std::function<void(Window* window, glm::vec2 size)>;
using ContentScaleFunc     = std::function<void(Window* window, glm::vec2 scale)>;
using MouseButtonFunc      = std::function<void(Window* window, int button, int action, int mods)>;
using CursorPosFunc        = std::function<void(Window* window, glm::vec2 pos)>;
using CursorEnterFunc      = std::function<void(Window* window, bool entered)>;
using ScrollFunc           = std::function<void(Window* window, glm::vec2 offset)>;
using KeyFunc              = std::function<void(Window* window, int key, int scancode, int action, int mods)>;
using CharFunc             = std::function<void(Window* window, unsigned int codepoint)>;
using CharModsFunc         = std::function<void(Window* window, unsigned int codepoint, int mods)>;
using DropFunc             = std::function<void(Window* window, int path_count, const char* paths[])>;

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
		const WindowProps& GetProperties() const { return m_Data; }
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