#pragma once

#include "AGI/Texture.hpp"

namespace AGI {

	class OpenGLTexture : public TextureBase
	{
	public:
		OpenGLTexture(TextureSpecification spec);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return m_Specification.Width; }
		virtual uint32_t GetHeight() const override { return m_Specification.Height; }
		virtual TextureSpecification GetSpecification() const override { return m_Specification; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture*)other.get())->m_RendererID;
		}
	private:
		TextureSpecification m_Specification;
		uint32_t m_RendererID;
	};

}