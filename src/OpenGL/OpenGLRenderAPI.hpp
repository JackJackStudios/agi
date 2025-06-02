#pragma once

#include "AGI/RenderContext.hpp"

namespace AGI {

	class OpenGLContext : public RenderContext
	{
	public:
		OpenGLContext(Settings settings);

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColour(const glm::vec4& colour) override;
		virtual void SetTextureAlignment(int align) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
	};


}