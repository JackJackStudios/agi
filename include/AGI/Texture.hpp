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
		glm::uvec2 Size;
		bool LinearFiltering = false;

		ImageFormat Format = ImageFormat::RGB;
		WrappingType Wrapping = WrappingType::Repeat;
		uint16_t BytesPerChannel = 8;

		void* Data = nullptr;
		uint32_t Datasize = 0;
	};

	class TextureBase : public RefCounted
	{
	public:
		virtual ~TextureBase() = default;

		virtual const glm::uvec2& GetSize() const = 0;
		virtual const TextureSpecification& GetSpecification() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	using Texture = ResourceBarrier<TextureBase>;

}