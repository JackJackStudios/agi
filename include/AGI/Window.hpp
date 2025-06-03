#pragma once

#include "RenderContext.hpp"
#include "Log.hpp"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace AGI {

	class Window
	{
	public:
		Window(const std::unique_ptr<RenderContext>& context, bool initContext);
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

		static float GetTime() { return glfwGetTime(); }
		static std::unique_ptr<Window> Create(const std::unique_ptr<RenderContext>& context, bool initContext = false) { return std::make_unique<Window>(context, initContext); }
	private:
		GLFWwindow* m_Window;
		WindowProps m_Data;
	};

}