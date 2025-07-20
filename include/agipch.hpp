#pragma once

#include <memory>
#include <functional>
#include <filesystem>
#include <string>
#include <format>
#include <vector>
#include <cstring>

#if defined(AGI_WINDOWS)
#define AGI_DEBUGBREAK() __debugbreak()
#elif defined(AGI_LINUX)
#include <signal.h>
#define AGI_DEBUGBREAK() raise(SIGTRAP)
#endif

#include "AGI/RenderContext.hpp"
#include "AGI/Log.hpp"

#include <glm/glm.hpp>


