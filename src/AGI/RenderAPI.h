#pragma once

#include "defines.h"

#include "VertexArray.h"

typedef void* (*GLADloadproc)(const char* name);

namespace AGI {

	class RenderAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual ~RenderAPI() = default;

		virtual void Init(GLADloadproc loadfunc) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;

		static API BestAPI() { return API::OpenGL; }
		static API GetAPI() { return s_API; }
		static Scope<RenderAPI> Create(API api);
	private:
		inline static API s_API;
	};

}