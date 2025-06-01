#include "agipch.hpp"
#include "AGI/Font.hpp"

namespace AGI {

	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static std::shared_ptr<AGI::Texture> CreateAndCacheAtlas(float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs,
		const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;

		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), (int)glyphs.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

		TextureSpecification textureSpec;
		textureSpec.Width = bitmap.width;
		textureSpec.Height = bitmap.height;
		textureSpec.Format = ImageFormat::RGB;
		textureSpec.LinearFiltering = true;

		std::shared_ptr<AGI::Texture> texture = Texture::Create(textureSpec);
		texture->SetData((void*)bitmap.pixels, bitmap.width * bitmap.height * ImageFormatToChannels(ImageFormat::RGB));
		
		return texture;
	}

	Font::Font(const std::filesystem::path& filepath)
	{
		msdfgen::FontHandle* font = msdfgen::loadFont(RenderAPI::GetCurrentAPI()->m_Freetype, filepath.string().c_str());
		if (!font)
		{
			AGI_ERROR("Failed to load font: {}", filepath.string());
			return;
		}

		msdf_atlas::Charset charset;
		for (uint32_t c = LatinCharacters.Begin; c <= LatinCharacters.End; c++)
			charset.add(c);

		double fontScale = 1.0;
		double atlasScale = 40.0;

		m_FontGeometry = msdf_atlas::FontGeometry(&m_Glyphs);
		int glyphsLoaded = m_FontGeometry.loadCharset(font, fontScale, charset);

		AGI_INFO("Loaded {} glyphs from font (out of {})", glyphsLoaded, charset.size());

		msdf_atlas::TightAtlasPacker atlasPacker;
		atlasPacker.setPixelRange(2.0);
		atlasPacker.setMiterLimit(1.0);
		atlasPacker.setPadding(0);
		atlasPacker.setScale(atlasScale);

		int remaining = atlasPacker.pack(m_Glyphs.data(), (int)m_Glyphs.size());
		AGI_VERIFY(remaining == 0, "Failed to pack glyphs into the atlas");

		int width, height;
		atlasPacker.getDimensions(width, height);
		atlasScale = atlasPacker.getScale();

		uint64_t glyphSeed = 0;
		for (msdf_atlas::GlyphGeometry& glyph : m_Glyphs)
		{
			glyphSeed *= 6364136223846793005ull;
			glyph.edgeColoring(msdfgen::edgeColoringInkTrap, 3.0, glyphSeed);
		}

		m_AtlasTexture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>((float)atlasScale, m_Glyphs, m_FontGeometry, width, height);

		msdfgen::destroyFont(font);
	}

	const msdf_atlas::GlyphGeometry* Font::TryGetGlyph(UniCode codepoint) const
	{
		auto glyph = m_FontGeometry.getGlyph(codepoint);
		if (!glyph) glyph = m_FontGeometry.getGlyph('?');

		if (!glyph)
		{
			AGI_WARN("No available glyph for character ({})", codepoint);
			return nullptr;
		}

		return glyph;
	}

	std::string Font::GetDefaultShader()
	{
		return R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			float median(float r, float g, float b) 
			{
				return max(min(r, g), min(max(r, g), b));
			}

			void main() 
			{
				const float pxRange = 2.0; // set to distance field's pixel range

				vec3 msd = texture(u_Texture, v_TexCoord).rgb;
				float sd = median(msd.r, msd.g, msd.b);
				float alpha = clamp((sd - 0.5) * pxRange + 0.5, 0.0, 1.0);

				color = vec4(1.0, 1.0, 1.0f, alpha);
			}
		)";
	}

}