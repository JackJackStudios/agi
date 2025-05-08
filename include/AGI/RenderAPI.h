#pragma once

#include "VertexArray.h"
#include "Log.h"

#include <glm/glm.hpp>

namespace AGI {

	enum class APIType
	{
		None = 0,
		Guess,
		OpenGL,
	};

	typedef void* (* OpenGLloaderFn)(const char *name);

	struct RenderAPISetttings
	{
		APIType PreferedAPI;
		OpenGLloaderFn loaderfunc;
		MessageCallbackFn messagefunc;
	};

	class RenderAPI
	{
	public:
		virtual ~RenderAPI() = default;
		
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) = 0;
		
		APIType GetType() const { return m_APIType; }

		static RenderAPI* GetCurrentAPI() { return s_CurrentAPI; }
		static std::unique_ptr<RenderAPI> Create(RenderAPISetttings settings);
	private:
		APIType m_APIType;
		RenderAPISetttings m_Settings;

		inline static RenderAPI* s_CurrentAPI;
	};

}