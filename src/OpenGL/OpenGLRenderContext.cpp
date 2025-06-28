#include "agipch.hpp"
#include "OpenGLRenderContext.hpp"

#include <glad/glad.h>

namespace AGI {

	void OpenGLContext::Init()
	{
		int	status = gladLoadGLLoader((GLADloadproc)m_Settings.LoaderFunc);
		AGI_VERIFY(status, "Failed to initialize Glad!");

		AGI_INFO("OpenGL Info:");
		AGI_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		AGI_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		AGI_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));

		if (m_Settings.Blending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
	}

	void OpenGLContext::Shutdown()
	{

	}

	void OpenGLContext::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLContext::SetClearColour(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void OpenGLContext::SetTextureAlignment(int align)
	{
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &align);
	}

	void OpenGLContext::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLContext::DrawIndexed(const VertexArray& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

}