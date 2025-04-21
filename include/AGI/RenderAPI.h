#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

typedef void* (*AGIloadproc)(const char* name);

namespace AGI {

	enum class API
	{
		None = 0, 
		OpenGL,
	};

	static API BestAPI()
	{
		return API::OpenGL;
	}

	enum class LogLevel
	{
		Trace = 1 << 0,
		Info = 1 << 1,
		Warning = 1 << 2,
		Error = 1 << 3,
		All = Trace | Info | Warning | Error
	};

	using MessageCallbackFn = std::function<void(std::string_view, LogLevel)>;
	
	class RenderAPI
	{
	public:
		virtual ~RenderAPI() = default;

		virtual void Init(AGIloadproc loadfunc) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) = 0;

		static void SetPrintCallback(MessageCallbackFn function) { s_LogCallback = function; }
		static void LogToConsole(std::string_view message, LogLevel level) { s_LogCallback(message, level); }

		static API GetAPI() { return s_CurrentAPI; }
		static std::unique_ptr<RenderAPI> Create(API api);
	private:
		inline static API s_CurrentAPI;
		inline static MessageCallbackFn s_LogCallback;
	};

}