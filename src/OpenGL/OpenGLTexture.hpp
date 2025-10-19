#pragma once

#include "AGI/Texture.hpp"

namespace AGI {

	class OpenGLTexture : public TextureBase
	{
	public:
		OpenGLTexture(TextureSpecification spec);
		virtual ~OpenGLTexture();

		virtual const glm::uvec2& GetSize() const override { return m_Specification.Size; }
		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot = 0) const override;
	private:
		TextureSpecification m_Specification;
		uint32_t m_RendererID;
	};

}