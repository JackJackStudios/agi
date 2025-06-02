#include "agipch.hpp"
#include "AGI/Window.hpp"

#include <GLFW/glfw3.h>

namespace AGI {

	static uint8_t s_GLFWWindowCount = 0;

	float Window::GetTime()
	{
		return (float)glfwGetTime();
	}

	static void GLFWErrorCallback(int error, const char* description)
	{
		AGI_ERROR("({0}): {1}", error, description);
	}

	Window::Window(const WindowProps& props)
		: m_Data(props)
	{
		AGI_INFO("Creating window \"{}\" ({}, {})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			AGI_VERIFY(success, "Could not intialize window system!");

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowHint(GLFW_VISIBLE, props.Visible);
		glfwWindowHint(GLFW_MAXIMIZED, props.Maximise);
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		++s_GLFWWindowCount;

		glfwMakeContextCurrent(m_Window);

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(m_Data.VSync);

		// Set GLFW callbacks
		//glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		//{
		//	WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
		//	data.Width = width;
		//	data.Height = height;
		//
		//	WindowResizeEvent event(width, height);
		//	Application::Get().OnEvent(event);
		//});
		//
		//glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		//{
		//	WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
		//	WindowCloseEvent event;
		//	Application::Get().OnEvent(event);
		//});
		//
		//glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		//{
		//	WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
		//
		//	MouseScrolledEvent event((float)xOffset, (float)yOffset);
		//	Application::Get().OnEvent(event);
		//});
		//
		//glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		//{
		//	WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
		//
		//	MouseMovedEvent event((float)xPos, (float)yPos);
		//	Application::Get().OnEvent(event);
		//});
		//
		//glfwSetDropCallback(m_Window, [](GLFWwindow* window, int pathCount, const char* paths[])
		//{
		//	WindowProps& data = *(WindowProps*)glfwGetWindowUserPointer(window);
		//
		//	std::vector<std::filesystem::path> filepaths(pathCount);
		//	for (int i = 0; i < pathCount; i++)
		//		filepaths[i] = paths[i];
		//
		//	WindowDropEvent event(std::move(filepaths));
		//	Application::Get().OnEvent(event);
		//});
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	glm::vec2 Window::GetPosition() const
	{
		int x;
		int y;
		glfwGetWindowPos(m_Window, &x, &y);

		return glm::vec2(x, y);
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}

	void Window::SetVisable(bool enabled)
	{
		if (enabled)
			glfwShowWindow(m_Window);
		else
			glfwHideWindow(m_Window);
	}

	void Window::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_Window, title.c_str());
		m_Data.Title = title;
	}

}