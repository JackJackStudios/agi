#pragma once

namespace AGI {

	enum class ImageFormat
	{
		None = 0, RED, RG, RGB, RGBA
	};

	ImageFormat ChannelsToImageFormat(uint16_t channels)
	{
		switch (channels)
		{
			case 0: return ImageFormat::None;
			case 1: return ImageFormat::RED;
			case 2: return ImageFormat::RG;
			case 3: return ImageFormat::RGB;
			case 4: return ImageFormat::RGBA;
		}

		AGI_WARN("Unknown channels count");
		return ImageFormat::None;
	}

	uint16_t ImageFormatToChannels(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::None: return 0;
			case ImageFormat::RED:  return 1;
			case ImageFormat::RG:   return 2;
			case ImageFormat::RGB:  return 3;
			case ImageFormat::RGBA: return 4;
		}

		AGI_WARN("Unknown ImageFormat");
		return 0;
	}

	struct TextureSpecifaction
	{
		uint32_t Width;
		uint32_t Height;
		bool LinearFiltering = false;
		ImageFormat Format = ImageFormat::RGB;
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

		static std::shared_ptr<Texture> Create(uint32_t width, uint32_t height, uint16_t channels);
		static std::shared_ptr<Texture> Create(const std::string& path);
	};

}