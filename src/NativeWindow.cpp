#include "agipch.hpp"
#include "AGI/Window.hpp"

#include <GLFW/glfw3.h>

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

void* AGI::Window::GetNativeWindow() const
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
