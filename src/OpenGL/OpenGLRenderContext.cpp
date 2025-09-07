#include "agipch.hpp"
#include "OpenGLRenderContext.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace AGI {

	bool OpenGLContext::Init()
	{
		m_BoundWindow->Init();

		int	status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (status == 0) return false;
		
		m_Properties.Renderer = (char*)glGetString(GL_RENDERER);
		m_Properties.Version = (char*)glGetString(GL_VERSION);
		m_Properties.Vendor = (char*)glGetString(GL_VENDOR);

		if (m_Settings.Blending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		PrintProperties();
		return true;
	}

	void OpenGLContext::Shutdown()
	{
		m_BoundWindow->Shutdown();
	}

	void OpenGLContext::BeginFrame()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLContext::EndFrame()
	{
		glfwSwapBuffers(m_BoundWindow->GetGlfwWindow());
	}

	void OpenGLContext::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLContext::SetClearColour(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void OpenGLContext::DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

}