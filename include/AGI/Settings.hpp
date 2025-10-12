#pragma once

namespace AGI {

	enum class APIType : uint32_t
	{
		OpenGL = 0, Vulkan, __COUNT
	};
	
	class Window;
	using WindowPosFunc = std::function<void(Window* window, glm::vec2 pos)>;
	using WindowSizeFunc = std::function<void(Window* window, glm::vec2 size)>;
	using WindowCloseFunc = std::function<void(Window* window)>;
	using WindowFocusFunc = std::function<void(Window* window, bool focused)>;
	using WindowMaximizeFunc = std::function<void(Window* window, bool maximized)>;
	using MouseButtonFunc = std::function<void(Window* window, int button, int action, int mods)>;
	using CursorPosFunc = std::function<void(Window* window, glm::vec2 pos)>;
	using ScrollFunc = std::function<void(Window* window, glm::vec2 offset)>;
	using KeyFunc = std::function<void(Window* window, int key, int scancode, int action, int mods)>;
	using CharFunc = std::function<void(Window* window, unsigned int codepoint)>;
	using DropFunc = std::function<void(Window* window, int path_count, const char* paths[])>;

	enum class WindowMode
	{
		Windowed = 0, Maximized, Fullscreen,
	};

	struct ContextProperties
	{
		std::string Vendor;
		std::string Version;
		std::string Renderer;
	};

	struct WindowProps
	{
		std::string Title = "AGI-Window";
		glm::uvec2 Size = { 1280, 720 };
		bool Visible = true;
		bool Resizable = true;
		bool Borderless = false;
		bool VSync = true;
		WindowMode Mode = WindowMode::Windowed;
	};

	struct Settings
	{
		APIType PreferedAPI;
		MessageCallbackFn MessageFunc;

		bool EnableValidation = true;
		bool Blending;
	};

	APIType BestAPI();

}