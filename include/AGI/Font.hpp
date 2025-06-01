#pragma once

#undef INFINITE
#include "msdf-atlas-gen/msdf-atlas-gen.h"

#include "AGI/Texture.hpp"

namespace AGI {

	struct CharsetRange
	{
		uint32_t Begin, End;
	};

	static constexpr CharsetRange LatinCharacters = { 0x0020, 0x00FF };

	class Font
	{
	public:
		Font(const std::filesystem::path& filepath);

		std::shared_ptr<AGI::Texture> GetAtlasTexture() const { return m_AtlasTexture; }
	private:
		std::shared_ptr<AGI::Texture> m_AtlasTexture;

		std::vector<msdf_atlas::GlyphGeometry> m_Glyphs;
		msdf_atlas::FontGeometry m_FontGeometry;
	};

}