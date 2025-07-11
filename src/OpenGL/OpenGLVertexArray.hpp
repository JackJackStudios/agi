#pragma once

#include "AGI/VertexArray.hpp"

namespace AGI {

	class OpenGLVertexArray : public VertexArrayBase
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(VertexBuffer& vertexBuffer) override;
		virtual void SetIndexBuffer(IndexBuffer& indexBuffer) override;

		virtual const std::vector<VertexBuffer>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const IndexBuffer& GetIndexBuffer() const override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<VertexBuffer> m_VertexBuffers;
		IndexBuffer m_IndexBuffer;
	};

}