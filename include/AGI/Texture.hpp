#pragma once

namespace AGI {

	enum class ImageFormat
	{
		RED = 0, RG, RGB, RGBA
	};

	enum class WrappingType
	{
		ClampBorder = 0, ClampEdge, Repeat, MirrorRepeat
	};

	namespace Utils {

		ImageFormat ChannelsToImageFormat(uint16_t channels);
		uint16_t ImageFormatToChannels(ImageFormat format);

	};

	struct TextureSpecification
	{
		uint32_t Width;
		uint32_t Height;

		bool Grayscale = false;
		bool LinearFiltering = false;

		ImageFormat Format = ImageFormat::RGB;
		WrappingType Wrapping = WrappingType::Repeat;
		uint16_t BytesPerChannel = 8;

		uint8_t* Data = nullptr;
		uint32_t Datasize = 0;
	};

	class TextureBase
	{
	public:
		virtual ~TextureBase() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual TextureSpecification GetSpecification() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const std::shared_ptr<TextureBase>& other) const = 0;
	};

	using Texture = std::shared_ptr<TextureBase>;

}