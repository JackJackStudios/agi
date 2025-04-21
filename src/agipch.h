#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <string>
#include <format>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
#define AGI_PLATFORM_WINDOWS
#else
	/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define AGI_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define AGI_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
#define AGI_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define AGI_PLATFORM_LINUX
#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection

#if defined(AGI_PLATFORM_WINDOWS)
#define AGI_DEBUGBREAK() __debugbreak()
#elif defined(AGI_PLATFORM_LINUX)
#include <signal.h>
#define AGI_DEBUGBREAK() raise(SIGTRAP)
#endif

#include "AGI/RenderAPI.h"

#define AGI_TRACE(...) ::AGI::RenderAPI::LogToConsole(std::format(__VA_ARGS__), ::AGI::LogLevel::Trace)
#define AGI_INFO(...)  ::AGI::RenderAPI::LogToConsole(std::format(__VA_ARGS__), ::AGI::LogLevel::Info)
#define AGI_WARN(...)  ::AGI::RenderAPI::LogToConsole(std::format(__VA_ARGS__), ::AGI::LogLevel::Warning)  
#define AGI_ERROR(...) ::AGI::RenderAPI::LogToConsole(std::format(__VA_ARGS__), ::AGI::LogLevel::Error); AGI_DEBUGBREAK()

#define AGI_VERIFY(x, ...) { if(!(x)) { AGI_ERROR(__VA_ARGS__); } }

#include <glm/glm.hpp>

