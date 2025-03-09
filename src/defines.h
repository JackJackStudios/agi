#pragma once

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

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#if defined(AGI_PLATFORM_WINDOWS)
#define AGI_DEBUGBREAK() __debugbreak()
#elif defined(AGI_PLATFORM_LINUX)
#include <signal.h>
#define AGI_DEBUGBREAK() raise(SIGTRAP)
#endif

#ifdef AGI_DEBUG
#define AGI_ENABLE_ASSERTS
#endif

namespace AGI {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// Non-owning raw buffer class
	struct Buffer
	{
		uint8_t* Data = nullptr;
		uint64_t Size = 0;

		Buffer() = default;

		Buffer(uint64_t size)
		{
			Allocate(size);
		}

		Buffer(const void* data, uint64_t size)
			: Data((uint8_t*)data), Size(size)
		{
		}

		Buffer(const Buffer&) = default;

		static Buffer Copy(Buffer other)
		{
			Buffer result(other.Size);
			memcpy(result.Data, other.Data, other.Size);
			return result;
		}

		void Allocate(uint64_t size)
		{
			Release();

			Data = (uint8_t*)malloc(size);
			Size = size;
		}

		void Release()
		{
			free(Data);
			Data = nullptr;
			Size = 0;
		}

		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		operator bool() const
		{
			return (bool)Data;
		}

	};
}

#define AGI_TRACE(...)   
#define AGI_INFO(...)    
#define AGI_WARN(...)    
#define AGI_ERROR(...)   
#define AGI_CRITICAL(...)

#ifdef AGI_ENABLE_ASSERTS
#define AGI_ASSERT(x, ...) { if(!(x)) { AGI_ERROR("Assertion Failed: {0}", __VA_ARGS__); AGI_DEBUGBREAK(); } }
#else
#define AGI_ASSERT(...)
#endif

#include <glm/glm.hpp>

