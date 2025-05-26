#pragma once

namespace AGI {

	enum class ImageFormat
	{
		None = 0, RED, RG, RGB, RGBA
	};

	enum class WrappingType
	{
		None = 0, ClampBorder, ClampEdge, Repeat, MirrorRepeat
	};

	ImageFormat ChannelsToImageFormat(uint16_t channels);
	uint16_t ImageFormatToChannels(ImageFormat format);

	struct TextureSpecifaction
	{
		uint32_t Width;
		uint32_t Height;
		bool LinearFiltering = false;
		ImageFormat Format = ImageFormat::RGB;
		WrappingType WrappingType = WrappingType::Repeat;
		uint16_t BytesPerChannel = 8;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual ImageFormat GetFormat() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;

		static std::shared_ptr<Texture> Create(TextureSpecifaction spec);
	};

}