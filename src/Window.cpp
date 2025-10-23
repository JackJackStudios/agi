#include "AGI/Window.hpp"
#include <GLFW/glfw3.h>

namespace AGI {

	static void GLFWErrorCallback(int error, const char* description)
	{
		AGI_ERROR("({}): {}", error, description);
	}

	static std::atomic<uint32_t> s_WindowCount = 0;
	static std::atomic<GLFWwindow*> s_LastWindow = nullptr;

	Window::Window(const Settings& settings, const WindowProps& props)
		: m_Settings(settings), m_Properties(props)
	{
		Log::Init(m_Settings.MessageFunc);

		if (s_WindowCount == 0)
		{
			int success = glfwInit();
			AGI_VERIFY(success, "Could not intialize window system!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_WindowIndex = s_WindowCount;
		++s_WindowCount;
	}

	void Window::Init()
	{
		glfwWindowHint(GLFW_RESIZABLE, m_Properties.Resizable);
		glfwWindowHint(GLFW_VISIBLE, m_Properties.Visible);
		glfwWindowHint(GLFW_DECORATED, !m_Properties.Borderless);
		glfwWindowHint(GLFW_MAXIMIZED, m_Properties.Mode == WindowMode::Maximized);
		glfwWindowHint(GLFW_CLIENT_API, (m_Settings.PreferedAPI == APIType::OpenGL ? GLFW_OPENGL_API : GLFW_NO_API));
		AGI_INFO("Creating window \"{}\" ({}, {})", m_Properties.Title, m_Properties.Size.x, m_Properties.Size.y);

		m_Window = glfwCreateWindow(m_Properties.Size.x, m_Properties.Size.y, m_Properties.Title.c_str(), m_Properties.Mode == WindowMode::Fullscreen ? glfwGetPrimaryMonitor() : nullptr, m_Settings.ShareResources ? s_LastWindow.load() : nullptr);
		CenterScreen();

		s_LastWindow = m_Window;

		if (m_Settings.PreferedAPI == APIType::OpenGL)
		{
			glfwMakeContextCurrent(m_Window);
			glfwSwapInterval((int)m_Properties.VSync);
		}

		glfwSetWindowUserPointer(m_Window, this);
		InstallCallbacks();
	}

	void Window::CenterScreen()
	{
		GLFWmonitor* primary = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(primary);

		int monitorX, monitorY;
		glfwGetMonitorPos(primary, &monitorX, &monitorY);

		int xpos = monitorX + (mode->width - m_Properties.Size.x) / 2;
		int ypos = monitorY + (mode->height - m_Properties.Size.y) / 2;
		glfwSetWindowPos(m_Window, xpos, ypos);
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
		--s_WindowCount;

		if (s_WindowCount == 0)
			glfwTerminate();
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	bool Window::ShouldClose(bool closing)
	{
		if (closing) glfwSetWindowShouldClose(m_Window, 1);
		return glfwWindowShouldClose(m_Window);
	}

	glm::vec2 Window::GetPosition() const
	{
		int x, y;
		glfwGetWindowPos(m_Window, &x, &y);

		return glm::vec2(x, y);
	}

	void Window::SetVisable(bool enabled)
	{
		if (enabled)
		{
			glfwShowWindow(m_Window);
		}
		else
		{
			glfwHideWindow(m_Window);
		}
	}

	void Window::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_Window, title.c_str());
		m_Properties.Title = title;
	}
	
	float Window::GetDelta()
	{
		float time = glfwGetTime();
		float timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;
		
		return timestep;
	}

    void Window::InstallCallbacks()
    {
		// Just try debugging this :)
		glfwSetWindowPosCallback(m_Window,          [](GLFWwindow* window, int xpos, int ypos)                          { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.WindowPosCallback)         { agiwindow->m_Events.WindowPosCallback(agiwindow, { xpos, ypos });         } });
		glfwSetWindowSizeCallback(m_Window,         [](GLFWwindow* window, int width, int height)                       { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.WindowSizeCallback)        { agiwindow->m_Events.WindowSizeCallback(agiwindow, { width, height }); } agiwindow->m_Properties.Size = { width, height }; });
		glfwSetWindowCloseCallback(m_Window,        [](GLFWwindow* window)                                              { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.WindowCloseCallback)       { agiwindow->m_Events.WindowCloseCallback(agiwindow);                       } });
		glfwSetWindowFocusCallback(m_Window,        [](GLFWwindow* window, int focused)                                 { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.WindowFocusCallback)       { agiwindow->m_Events.WindowFocusCallback(agiwindow, (bool)focused);        } });
		glfwSetWindowMaximizeCallback(m_Window,     [](GLFWwindow* window, int maximsed)                                { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.WindowMaximizeCallback)    { agiwindow->m_Events.WindowMaximizeCallback(agiwindow, (bool)maximsed);    } });
		glfwSetMouseButtonCallback(m_Window,        [](GLFWwindow* window, int button, int action, int mods)            { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.MouseButtonCallback)       { agiwindow->m_Events.MouseButtonCallback(agiwindow, button, action, mods); } });
		glfwSetCursorPosCallback(m_Window,          [](GLFWwindow* window, double xpos, double ypos)                    { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.CursorPosCallback)         { agiwindow->m_Events.CursorPosCallback(agiwindow, { xpos, ypos });         } });
		glfwSetScrollCallback(m_Window,             [](GLFWwindow* window, double xoffset, double yoffset)              { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.ScrollCallback)            { agiwindow->m_Events.ScrollCallback(agiwindow, { xoffset, yoffset });      } });
		glfwSetKeyCallback(m_Window,                [](GLFWwindow* window, int key, int scancode, int action, int mods) { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.KeyCallback)               { agiwindow->m_Events.KeyCallback(agiwindow, key, scancode, action, mods);  } });
		glfwSetCharCallback(m_Window,               [](GLFWwindow* window, unsigned int codepoint)                      { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.CharCallback)              { agiwindow->m_Events.CharCallback(agiwindow, codepoint);                   } });
		glfwSetDropCallback(m_Window,               [](GLFWwindow* window, int path_count, const char* paths[])         { Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); if (agiwindow->m_Events.DropCallback)              { agiwindow->m_Events.DropCallback(agiwindow, path_count, paths);           } });
    }
    
}
