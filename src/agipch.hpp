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

#if defined(AGI_WINDOWS)
#define AGI_DEBUGBREAK() __debugbreak()
#elif defined(AGI_LINUX)
#include <signal.h>
#define AGI_DEBUGBREAK() raise(SIGTRAP)
#endif

#include "AGI/RenderAPI.h"
#include "AGI/Log.h"

#include <glm/glm.hpp>


