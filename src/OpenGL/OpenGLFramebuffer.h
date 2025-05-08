#pragma once

#include "agi/Framebuffer.h"

namespace AGI {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(uint32_t width, uint32_t height);
		virtual ~OpenGLFramebuffer();
		
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(int x, int y) override;

		virtual int GetWidth() override { return m_Width; };
		virtual int GetHeight() override { return m_Height; };

		virtual uint32_t GetAttachmentID() override { return m_ColourAttachment; };
	private:
		void Invalidate();
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColourAttachment = 0;

		uint32_t m_Width;
		uint32_t m_Height;
	};

}