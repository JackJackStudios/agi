#pragma once

#include "VertexArray.h"
#include "Log.h"

#include <glm/glm.hpp>

namespace AGI {

	enum class GraphicsEngine
	{
		None = 0, 
		OpenGL,
	};

	static GraphicsEngine BestAPI();

	typedef void* (* OpenGLloaderFn)(const char *name);

	struct RenderAPISetttings
	{
		OpenGLloaderFn loaderfunc;
		MessageCallbackFn messagefunc;
	};

	class RenderAPI
	{
	public:
		virtual ~RenderAPI() = default;

		virtual void Init(RenderAPISetttings settings) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) = 0;
		
		GraphicsEngine GetGraphicsEngine() const { return m_GraphicsEngine; }

		static GraphicsEngine GetAPI() { return s_CurrentAPI->GetGraphicsEngine(); }
		static std::unique_ptr<RenderAPI> Create(GraphicsEngine api);
	private:
		GraphicsEngine m_GraphicsEngine;

		inline static RenderAPI* s_CurrentAPI;
	};

}