#pragma once

namespace AGI {

	enum class APIType
	{
		Guess = 0,
		OpenGL,
		//Vulkan, Coming soon :)
	};
	
	class Window;
	using WindowPosFunc = std::function<void(Window* window, glm::vec2 pos)>;
	using WindowSizeFunc = std::function<void(Window* window, glm::vec2 size)>;
	using WindowCloseFunc = std::function<void(Window* window)>;
	using WindowRefreshFunc = std::function<void(Window* window)>;
	using WindowFocusFunc = std::function<void(Window* window, bool focused)>;
	using WindowIconifyFunc = std::function<void(Window* window, bool iconified)>;
	using WindowMaximizeFunc = std::function<void(Window* window, bool maximized)>;
	using FramebufferSizeFunc = std::function<void(Window* window, glm::vec2 size)>;
	using ContentScaleFunc = std::function<void(Window* window, glm::vec2 scale)>;
	using MouseButtonFunc = std::function<void(Window* window, int button, int action, int mods)>;
	using CursorPosFunc = std::function<void(Window* window, glm::vec2 pos)>;
	using CursorEnterFunc = std::function<void(Window* window, bool entered)>;
	using ScrollFunc = std::function<void(Window* window, glm::vec2 offset)>;
	using KeyFunc = std::function<void(Window* window, int key, int scancode, int action, int mods)>;
	using CharFunc = std::function<void(Window* window, unsigned int codepoint)>;
	using CharModsFunc = std::function<void(Window* window, unsigned int codepoint, int mods)>;
	using DropFunc = std::function<void(Window* window, int path_count, const char* paths[])>;

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

	struct Settings
	{
		APIType PreferedAPI;
		MessageCallbackFn MessageFunc;
		WindowProps Window;
		bool Blending;
	};

	APIType BestAPI();
	APIType ActualAPI(APIType* type);

}