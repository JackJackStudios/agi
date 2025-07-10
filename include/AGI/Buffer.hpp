#pragma once

#include "AGI/Log.hpp"

namespace AGI {

	enum class ShaderDataType
	{
		Float = 0, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	namespace Utils {

		static uint32_t ShaderDataTypeSize(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
			case ShaderDataType::Int:      return 4;
			case ShaderDataType::Int2:     return 4 * 2;
			case ShaderDataType::Int3:     return 4 * 3;
			case ShaderDataType::Int4:     return 4 * 4;
			case ShaderDataType::Bool:     return 1;
			}

			return 0;
		}

	};

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(Utils::ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Float2:  return 2;
				case ShaderDataType::Float3:  return 3;
				case ShaderDataType::Float4:  return 4;
				case ShaderDataType::Mat3:    return 3; // 3 * float3
				case ShaderDataType::Mat4:    return 4; // 4 * float4
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::Int2:    return 2;
				case ShaderDataType::Int3:    return 3;
				case ShaderDataType::Int4:    return 4;
				case ShaderDataType::Bool:    return 1;
			}

			return 0;
		}

		bool operator==(const BufferElement& other) const {
			return Name == other.Name &&
				Type == other.Type &&
				Size == other.Size &&
				Offset == other.Offset;
		}

	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::vector<BufferElement> elements)
			: m_Elements(elements)
		{
			SetOffsets();
		}

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			SetOffsets();
		}

		void SetOffsets()
		{
			for (auto& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}

		uint32_t GetStride() const { return m_Stride; }
		size_t GetSize() const { return m_Elements.size(); }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		void PushBack(BufferElement&& element)
		{
			element.Offset = m_Stride;
			m_Stride += element.Size;
			m_Elements.emplace_back(element);
		}

		bool HasElement(const std::string& name) const
		{
			return std::any_of(begin(), end(), [&](const BufferElement& e) {
        		return e.Name == name;
    		});
		};

		BufferElement& GetElement(const std::string& name)
		{
			auto it = std::find_if(begin(), end(), [&](const BufferElement& e) {
				return e.Name == name;
			});
		
			return *it;
		}

		BufferElement& operator[](int idx)
		{
			AGI_VERIFY(idx < m_Elements.size(), "idx out of range for BufferElement");
			return m_Elements[idx];
		}

		BufferElement& operator[](const std::string& name)
		{
			AGI_VERIFY(HasElement(name), "Element \"{}\" does not exist", name);
			return GetElement(name);
		}

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBufferBase
	{
	public:
		virtual ~VertexBufferBase() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual uint32_t GetSize() const = 0;
	};

	using VertexBuffer = std::shared_ptr<VertexBufferBase>;

	class IndexBufferBase
	{
	public:
		virtual ~IndexBufferBase() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;
	};

	using IndexBuffer = std::shared_ptr<IndexBufferBase>;

}