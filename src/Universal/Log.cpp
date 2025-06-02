#include "agipch.hpp"
#include "AGI/Log.hpp"

namespace AGI {

    void Log::Init(MessageCallbackFn logfunc)
    {
        if (!IsInitialized())
            s_LogCallback = logfunc;
    }

    void Log::LogToClient(std::string_view message, LogLevel level)
    {
        s_LogCallback(message, level);
    }

    void Log::LogToLibrary(std::string_view message, LogLevel level)
    {
        std::cout << "AGI: " << message << std::endl;
    }
}