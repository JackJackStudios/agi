#pragma once

namespace AGI {

	enum class FramebufferTextureFormat
	{
		None = 0,

		// Color
		RGBA8,
		RED_INTEGER,
	};

	struct FramebufferSpecification
	{
		uint32_t Width = 0, Height = 0;
		std::vector<FramebufferTextureFormat> Attachments;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t index, uint32_t x, uint32_t y) = 0;
		
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		
		virtual int GetWidth() = 0;
		virtual int GetHeight() = 0;

		virtual uint32_t GetAttachmentID(uint32_t index = 0) = 0;
		
		static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification& spec);
	};

}