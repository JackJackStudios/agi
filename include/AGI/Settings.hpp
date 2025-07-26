#pragma once

namespace AGI {

	enum class APIType
	{
		OpenGL = 0,
	};
	
	class Window;
	using WindowPosFunc = std::function<void(Window* window, glm::vec2 pos)>;
	using WindowSizeFunc = std::function<void(Window* window, glm::vec2 size)>;
	using WindowCloseFunc = std::function<void(Window* window)>;
	using WindowFocusFunc = std::function<void(Window* window, bool focused)>;
	using WindowMaximizeFunc = std::function<void(Window* window, bool maximized)>;
	using MouseButtonFunc = std::function<void(Window* window, int button, int action, int mods)>;
	using CursorPosFunc = std::function<void(Window* window, glm::vec2 pos)>;
	using CursorEnterFunc = std::function<void(Window* window, bool entered)>;
	using ScrollFunc = std::function<void(Window* window, glm::vec2 offset)>;
	using KeyFunc = std::function<void(Window* window, int key, int scancode, int action, int mods)>;
	using CharFunc = std::function<void(Window* window, unsigned int codepoint)>;
	using DropFunc = std::function<void(Window* window, int path_count, const char* paths[])>;

	struct WindowProps
	{
		std::string Title = "AGI-Window";
		uint32_t Width = 1280;
		uint32_t Height = 720;
		bool VSync = true;
		bool Resize = true;
		bool Visible = true;
		bool Decorated = true;
		bool Maximise = false;
	};

	struct Settings
	{
		APIType PreferedAPI;
		MessageCallbackFn MessageFunc;
		WindowProps Window;
		bool Blending;
	};

	APIType BestAPI();

}