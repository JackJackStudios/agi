#pragma once

#include "Buffer.hpp"

namespace AGI {

	class VertexArrayBase
	{
	public:
		virtual ~VertexArrayBase() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(VertexBuffer& vertexBuffer) = 0;
		virtual void SetIndexBuffer(IndexBuffer& indexBuffer) = 0;

		virtual const std::vector<VertexBuffer>& GetVertexBuffers() const = 0;
		virtual const IndexBuffer& GetIndexBuffer() const = 0;
	};

	using VertexArray = std::shared_ptr<VertexArrayBase>;

}