#pragma once

#include <iostream>

namespace AGI {

    enum class LogLevel
	{
		Trace = 1 << 0,
		Info = 1 << 1,
		Warning = 1 << 2,
		Error = 1 << 3,
	};

	using MessageCallbackFn = std::function<void(std::string_view, LogLevel)>;

    class Log
    {
    public:
        static void Init(MessageCallbackFn logfunc) { if (!IsInitialized()) s_LogCallback = logfunc; }
        
        static void LogToClient(std::string_view message, LogLevel level) { s_LogCallback(message, level); }
        static void LogToLibrary(std::string_view message, LogLevel level)
        {
            switch (level)
            {
            case AGI::LogLevel::Trace:   std::cout << "[TRACE] AGI: " << message << std::endl; break;
            case AGI::LogLevel::Info:    std::cout << "[INFO] AGI: " << message << std::endl; break;
            case AGI::LogLevel::Warning: std::cout << "[WARN] AGI: " << message << std::endl; break;
            case AGI::LogLevel::Error:   std::cout << "[ERROR] AGI: " << message << std::endl; break;
            }
        }
        
        static void GenericLog(std::string_view message, LogLevel level) { if (!IsInitialized()) LogToLibrary(message, level); else LogToClient(message, level); }

        static bool IsInitialized() { return (bool)s_LogCallback; }
    private:
		inline static MessageCallbackFn s_LogCallback;
    };

    #define AGI_TRACE(...) ::AGI::Log::GenericLog(std::format(__VA_ARGS__), ::AGI::LogLevel::Trace)
    #define AGI_INFO(...)  ::AGI::Log::GenericLog(std::format(__VA_ARGS__), ::AGI::LogLevel::Info)
    #define AGI_WARN(...)  ::AGI::Log::GenericLog(std::format(__VA_ARGS__), ::AGI::LogLevel::Warning)  
    #define AGI_ERROR(...) ::AGI::Log::GenericLog(std::format(__VA_ARGS__), ::AGI::LogLevel::Error); AGI_DEBUGBREAK()
    
    #define AGI_VERIFY(x, ...) { if(!(x)) { AGI_ERROR(__VA_ARGS__); } }

}