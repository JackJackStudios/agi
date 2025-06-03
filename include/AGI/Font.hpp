#pragma once

#undef INFINITE
#include "msdf-atlas-gen/msdf-atlas-gen.h"

#include "AGI/Texture.hpp"
#include "AGI/Shader.hpp"

namespace AGI {

	struct CharsetRange
	{
		uint32_t Begin, End;
	};

	static constexpr CharsetRange LatinCharacters = { 0x0020, 0x00FF };

	using UniCode = uint32_t;

	class Font
	{
	public:
		Font(const std::filesystem::path& filepath);

		const msdf_atlas::GlyphGeometry* TryGetGlyph(UniCode codepoint) const;

		const msdfgen::FontMetrics& GetMetrics() const { return m_FontGeometry.getMetrics(); }
		std::shared_ptr<AGI::Texture> GetAtlasTexture() const { return m_AtlasTexture; }

		static std::string GetDefaultShader();
	private:
		std::shared_ptr<AGI::Texture> m_AtlasTexture;

		std::vector<msdf_atlas::GlyphGeometry> m_Glyphs;
		msdf_atlas::FontGeometry m_FontGeometry;
	};

}