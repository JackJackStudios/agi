#pragma once

#include "agi/Texture.h"

#include <glad/glad.h>

namespace AGI {

	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(TextureSpecifaction spec);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return m_Specifaction.Width; }
		virtual uint32_t GetHeight() const override { return m_Specifaction.Height; }
		virtual ImageFormat GetFormat() const override { return m_Specifaction.Format; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture&)other).m_RendererID;
		}
	private:
		TextureSpecifaction m_Specifaction;
		uint32_t m_RendererID;
	};

}