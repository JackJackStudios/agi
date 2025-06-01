#pragma once

#include "VertexArray.hpp"
#include "Log.hpp"

#include <glm/glm.hpp>

#undef INFINITE
#include "msdf-atlas-gen/msdf-atlas-gen.h"

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
		bool Blending;
		OpenGLloaderFn LoaderFunc;
		MessageCallbackFn MessageFunc;
	};

	class RenderAPI
	{
	public:
		virtual ~RenderAPI() = default;
		
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void SetTextureAlignment(int align) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		
		APIType GetType() const { return m_APIType; }

		static std::unique_ptr<RenderAPI> Init(RenderAPISetttings settings);
		static void Shutdown(std::unique_ptr<RenderAPI>& api);
		static RenderAPI* GetCurrentAPI() { return s_CurrentAPI; }
	private:
		APIType m_APIType;
		RenderAPISetttings m_Settings;
		msdfgen::FreetypeHandle* m_Freetype;

		inline static RenderAPI* s_CurrentAPI;

		friend class Font;
	};
}