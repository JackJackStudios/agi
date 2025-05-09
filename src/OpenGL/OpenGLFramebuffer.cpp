#include "agipch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace AGI {

	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specifation(spec)
	{
		if (spec.Attachments.size() == 0)
			AGI_ERROR("Cannot create Framebuffer with no attachments");

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColourAttachments.size(), m_ColourAttachments.data());
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColourAttachments.size(), m_ColourAttachments.data());
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		
		m_ColourAttachments.reserve(m_Specifation.Attachments.size());
		glGenTextures(m_ColourAttachments.size(), m_ColourAttachments.data());

		for (size_t i=0; i<m_ColourAttachments.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_ColourAttachments[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specifation.Width, m_Specifation.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_ColourAttachments[i], 0);
		}

		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(m_ColourAttachments.size(), buffers);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			AGI_ERROR("Framebuffer is incomplete!");
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specifation.Width, m_Specifation.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (m_Specifation.Width == width && m_Specifation.Height == height) return;
		
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			AGI_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}
		
		m_Specifation.Width = width;
		m_Specifation.Height = height;

		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t index, uint32_t x, uint32_t y)
	{
		int pixelData;
		glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}

}