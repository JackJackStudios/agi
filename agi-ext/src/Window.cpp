#include "AGI/Window.hpp"

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

#define SET_CALLBACK(NAME, ARGS, ...) \
	glfwSet##NAME(m_Window, []ARGS{ \
		Window* agiwindow = (Window*)glfwGetWindowUserPointer(window); \
		if (agiwindow->m_Data.NAME) agiwindow->m_Data.NAME(agiwindow, __VA_ARGS__); \
	})

namespace AGI {

	namespace Utils {

		static int AgiApiTypeToGlfwApiType(APIType type)
		{
			switch (type)
			{
				case APIType::Headless: return GLFW_NO_API;
				case APIType::OpenGL: return GLFW_OPENGL_API;
			}

			AGI_VERIFY(type != APIType::Guess, "APIType::Guess should not reach this function");

			AGI_VERIFY(false, "Undefined APIType");
			return 0;
		}

	}

	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		AGI_ERROR("({0}): {1}", error, description);
	}

	AGIWindow::Window(const WindowProps& windowProps, const std::unique_ptr<RenderContext>& context)
		: m_Data(windowProps)
	{
		AGI_INFO("Creating window \"{}\" ({}, {})", m_Data.Title, m_Data.Width, m_Data.Height);

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			AGI_VERIFY(success, "Could not intialize window system!");

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowHint(GLFW_RESIZABLE, m_Data.Resize);
		glfwWindowHint(GLFW_VISIBLE, m_Data.Visible);
		glfwWindowHint(GLFW_DECORATED, m_Data.Decorated);
		glfwWindowHint(GLFW_MAXIMIZED, m_Data.Maximise);
		glfwWindowHint(GLFW_CLIENT_API, Utils::AgiApiTypeToGlfwApiType(context->GetType()));
		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		glfwMakeContextCurrent(m_Window);

		glfwSetWindowUserPointer(m_Window, this);
		SetVSync(m_Data.VSync);

		// Set GLFW callbacks
		InstallCallbacks();
	}

	AGIWindow::~Window()
	{
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void AGIWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
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
		if (m_Data.VSync != enabled)
			glfwSwapInterval((int)enabled);

		m_Data.VSync = enabled;
	}

	bool AGIWindow::IsVSync() const
	{
		return m_Data.VSync;
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
		m_Data.Title = title;
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
		SET_CALLBACK(WindowPosCallback, (GLFWwindow* window, int xpos, int ypos), { xpos, ypos });
		SET_CALLBACK(WindowSizeCallback, (GLFWwindow* window, int width, int height), { width, height });
		SET_CALLBACK(WindowCloseCallback, (GLFWwindow* window));
		SET_CALLBACK(WindowRefreshCallback, (GLFWwindow* window));
		SET_CALLBACK(WindowFocusCallback, (GLFWwindow* window, int focused), (bool)focused);
		SET_CALLBACK(WindowIconifyCallback, (GLFWwindow* window, int iconifed), (bool)iconifed);
		SET_CALLBACK(WindowMaximizeCallback, (GLFWwindow* window, int maximsed), (bool)maximsed);
		SET_CALLBACK(FramebufferSizeCallback, (GLFWwindow* window, int width, int height), { width, height });
		SET_CALLBACK(WindowContentScaleCallback, (GLFWwindow* window, float xscale, float yscale), { xscale, yscale });
		SET_CALLBACK(MouseButtonCallback, (GLFWwindow* window, int button, int action, int mods), button, action, mods);
		SET_CALLBACK(CursorPosCallback, (GLFWwindow* window, double xpos, double ypos), { xpos, ypos });
		SET_CALLBACK(CursorEnterCallback, (GLFWwindow* window, int entered), (bool)entered);
		SET_CALLBACK(ScrollCallback, (GLFWwindow* window, double xoffset, double yoffset), { xoffset, yoffset });
		SET_CALLBACK(KeyCallback, (GLFWwindow* window, int key, int scancode, int action, int mods), key, scancode, action, mods);
		SET_CALLBACK(CharCallback, (GLFWwindow* window, unsigned int codepoint), codepoint);
		SET_CALLBACK(CharModsCallback, (GLFWwindow* window, unsigned int codepoint, int mods), codepoint, mods);
		SET_CALLBACK(DropCallback, (GLFWwindow* window, int path_count, const char *paths[]), path_count, paths);
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