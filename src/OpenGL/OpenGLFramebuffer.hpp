#pragma once

#include "AGI/Framebuffer.hpp"

namespace AGI {

	class OpenGLFramebuffer : public FramebufferBase
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();
		
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t index, uint32_t x, uint32_t y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual int GetWidth() override { return m_Specifation.Width; };
		virtual int GetHeight() override { return m_Specifation.Height; };

		virtual uint32_t GetAttachmentID(uint32_t index = 0) override { return m_ColourAttachments[index]; };
	private:
		void Invalidate();
	private:
		uint32_t m_RendererID = 0;

		FramebufferSpecification m_Specifation;
		std::vector<uint32_t> m_ColourAttachments;
	};

}