#include "AGI/Window.hpp"

#include <atomic>

using AGIWindow = AGI::Window;

#if defined(AGI_WINDOWS)
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
#elif defined(AGI_MACOSX)
    #define GLFW_EXPOSE_NATIVE_COCOA
    #include <GLFW/glfw3native.h>
#elif defined(AGI_LINUX)
    #define GLFW_EXPOSE_NATIVE_X11
    #define GLFW_EXPOSE_NATIVE_WAYLAND
    #include <GLFW/glfw3native.h>
#endif

namespace AGI {

	namespace Utils {

		static int AgiApiTypeToGlfwApiType(APIType type)
		{
			switch (type)
			{
			case APIType::OpenGL: return GLFW_OPENGL_API; break;

			default: AGI_VERIFY(false, "Undefined APIType"); break;
			}

			return 0;
		}

	}

	static void GLFWErrorCallback(int error, const char* description)
	{
		AGI_ERROR("({0}): {1}", error, description);
	}

	static std::atomic<uint32_t> s_WindowCount = 0;
	static std::atomic<bool> s_GLFWinit = false;

	// Main Thread
	AGIWindow::Window(Settings& settings)
		: m_Settings(settings)
	{
		Log::Init(m_Settings.MessageFunc);

		if (s_GLFWinit)
			return;
			
		int success = glfwInit();
		s_GLFWinit = true;

		AGI_VERIFY(success, "Could not intialize window system!");
		glfwSetErrorCallback(GLFWErrorCallback);
	}

	void AGIWindow::Init()
	{
		if (m_WindowIndex != -1)
			return;

		AGI_INFO("Creating window \"{}\" ({}, {})", m_Settings.Window.Title, m_Settings.Window.Width, m_Settings.Window.Height);

		glfwWindowHint(GLFW_RESIZABLE, m_Settings.Window.Resize);
		glfwWindowHint(GLFW_VISIBLE, m_Settings.Window.Visible);
		glfwWindowHint(GLFW_DECORATED, m_Settings.Window.Decorated);
		glfwWindowHint(GLFW_MAXIMIZED, m_Settings.Window.Maximise);
		glfwWindowHint(GLFW_CLIENT_API, Utils::AgiApiTypeToGlfwApiType(m_Settings.PreferedAPI));
		m_Window = glfwCreateWindow((int)m_Settings.Window.Width, (int)m_Settings.Window.Height, m_Settings.Window.Title.c_str(), nullptr, nullptr);
		s_WindowCount++;

		m_WindowIndex = s_WindowCount-1;
		
		glfwSetWindowUserPointer(m_Window, this);

		if (m_Settings.PreferedAPI == APIType::OpenGL)
			glfwMakeContextCurrent(m_Window);

		SetVSync(m_Settings.Window.VSync);
		
		// Set GLFW callbacks
		InstallCallbacks();
	}

	// Main Thread
	void AGIWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);

		s_WindowCount--;

		if (s_WindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void AGIWindow::OnUpdate()
	{
		if (m_Settings.PreferedAPI == APIType::OpenGL)
			glfwSwapBuffers(m_Window);

		glfwPollEvents();
	}

	bool AGIWindow::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	glm::vec2 AGIWindow::GetPosition() const
	{
		int x, y;
		glfwGetWindowPos(m_Window, &x, &y);

		return glm::vec2(x, y);
	}

	void AGIWindow::SetVSync(bool enabled)
	{
		if (m_Settings.Window.VSync != enabled)
			glfwSwapInterval((int)enabled);

		m_Settings.Window.VSync = enabled;
	}

	bool AGIWindow::IsVSync() const
	{
		return m_Settings.Window.VSync;
	}

	void AGIWindow::SetVisable(bool enabled)
	{
		if (enabled)
			glfwShowWindow(m_Window);
		else
			glfwHideWindow(m_Window);
	}

	void AGIWindow::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_Window, title.c_str());
		m_Settings.Window.Title = title;
	}

    bool AGIWindow::IsKeyOn(int32_t key) const
    {
		return glfwGetKey(m_Window, key) == GLFW_PRESS;
    }

    bool AGIWindow::IsMouseButtonOn(int32_t button) const
    {
		return glfwGetMouseButton(m_Window, button) == GLFW_PRESS;
    }

    glm::vec2 AGIWindow::GetCursorPos() const
    {
		double xpos, ypos;
		glfwGetCursorPos(m_Window, &xpos, &ypos);

		return glm::vec2(xpos, ypos);
    }

    void AGIWindow::InstallCallbacks()
    {
		// Just try debugging this :)
		glfwSetWindowPosCallback(m_Window,          [](GLFWwindow* window, int xpos, int ypos)                          { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.WindowPosCallback)          agiwindow->m_Settings.Window.Events.WindowPosCallback(agiwindow, { xpos, ypos }); });
		glfwSetWindowSizeCallback(m_Window,         [](GLFWwindow* window, int width, int height)                       { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.WindowSizeCallback)         agiwindow->m_Settings.Window.Events.WindowSizeCallback(agiwindow, { width, height }); });
		glfwSetWindowCloseCallback(m_Window,        [](GLFWwindow* window)                                              { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.WindowCloseCallback)        agiwindow->m_Settings.Window.Events.WindowCloseCallback(agiwindow); });
		glfwSetWindowFocusCallback(m_Window,        [](GLFWwindow* window, int focused)                                 { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.WindowFocusCallback)        agiwindow->m_Settings.Window.Events.WindowFocusCallback(agiwindow, (bool)focused); });
		glfwSetWindowMaximizeCallback(m_Window,     [](GLFWwindow* window, int maximsed)                                { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.WindowMaximizeCallback)     agiwindow->m_Settings.Window.Events.WindowMaximizeCallback(agiwindow, (bool)maximsed); });
		glfwSetMouseButtonCallback(m_Window,        [](GLFWwindow* window, int button, int action, int mods)            { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.MouseButtonCallback)        agiwindow->m_Settings.Window.Events.MouseButtonCallback(agiwindow, button, action, mods); });
		glfwSetCursorPosCallback(m_Window,          [](GLFWwindow* window, double xpos, double ypos)                    { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.CursorPosCallback)          agiwindow->m_Settings.Window.Events.CursorPosCallback(agiwindow, { xpos, ypos }); });
		glfwSetCursorEnterCallback(m_Window,        [](GLFWwindow* window, int entered)                                 { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.CursorEnterCallback)        agiwindow->m_Settings.Window.Events.CursorEnterCallback(agiwindow, (bool)entered); });
		glfwSetScrollCallback(m_Window,             [](GLFWwindow* window, double xoffset, double yoffset)              { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.ScrollCallback)             agiwindow->m_Settings.Window.Events.ScrollCallback(agiwindow, { xoffset, yoffset }); });
		glfwSetKeyCallback(m_Window,                [](GLFWwindow* window, int key, int scancode, int action, int mods) { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.KeyCallback)                agiwindow->m_Settings.Window.Events.KeyCallback(agiwindow, key, scancode, action, mods); });
		glfwSetCharCallback(m_Window,               [](GLFWwindow* window, unsigned int codepoint)                      { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.CharCallback)               agiwindow->m_Settings.Window.Events.CharCallback(agiwindow, codepoint); });
		glfwSetDropCallback(m_Window,               [](GLFWwindow* window, int path_count, const char* paths[])         { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Settings.Window.Events.DropCallback)               agiwindow->m_Settings.Window.Events.DropCallback(agiwindow, path_count, paths); });
    }

    void* AGIWindow::GetNativeWindow() const
    {
#if defined(AGI_WINDOWS)
    	return glfwGetWin32Window(m_Window);

#elif defined(AGI_MACOSX)
    	return glfwGetCocoaWindow(m_Window);

#elif defined(AGI_LINUX)
		if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) 
		{
			return (void*)glfwGetWaylandWindow(m_Window); // wl_surface*
		} 
		else if (glfwGetPlatform() == GLFW_PLATFORM_X11) 
		{
			return (void*)(uintptr_t)glfwGetX11Window(m_Window); // cast X11 Window (unsigned long) to void*
		} 
		else 
		{
			return nullptr; // Unknown or unsupported backend
		}
#else
    	return nullptr; // Unsupported platform
#endif
    }
}