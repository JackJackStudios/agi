#pragma once

#include <atomic>
#include <format>
#include <cstring>
#include <array>
#include <functional>

#if defined(AGI_WINDOWS)
#define AGI_DEBUGBREAK() __debugbreak()
#elif defined(AGI_LINUX)
#include <signal.h>
#define AGI_DEBUGBREAK() raise(SIGTRAP)
#endif

#include "AGI/ResourceBarrier.hpp"
#include "AGI/RenderContext.hpp"
#include "AGI/Log.hpp"

#include <glm/glm.hpp>
