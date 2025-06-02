#pragma once

#include "agipch.hpp"

#include "RenderContext.hpp"
#include "Log.hpp"

#include <glm/glm.hpp>

struct GLFWwindow;

namespace AGI {

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		Window(const WindowProps& props);
		~Window();

		void OnUpdate();

		inline uint32_t GetWidth() const { return m_Data.Width; }
		inline uint32_t GetHeight() const { return m_Data.Height; }
		glm::vec2 GetPosition() const;

		// Window attributes
		bool ShouldClose() const;
		bool IsVSync() const;

		void SetVSync(bool enabled);
		void SetVisable(bool enabled);
		void SetTitle(const std::string& title);

		GLFWwindow* GetNativeWindow() const { return m_Window; }

		static float GetTime();
		static std::unique_ptr<Window> Create(const Settings& settings) { Log::Init(settings.MessageFunc); return std::make_unique<Window>(settings.WindowProps); }
	private:
		GLFWwindow* m_Window;
		WindowProps m_Data;
	};

}