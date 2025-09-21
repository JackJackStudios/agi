cmake_minimum_required(VERSION 3.15)
project(AGI VERSION 0.4 LANGUAGES C CXX)

message(STATUS "AGI: Version ${PROJECT_VERSION}")

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

option(AGI_EXAMPLES "Build AGI examples and it's dependencies" ${MAIN_PROJECT})
option(AGI_EXTENSIONS "Use extensions and download it's dependencies" ON)

add_subdirectory(src)

if (AGI_EXAMPLES)
    add_subdirectory(examples)
endif()

if (AGI_EXTENSIONS)
    add_subdirectory(src/ext)
endif()


FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/icaven/glm.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(GLFW)

FetchContent_Declare(
    VulkanHeaders
    GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Headers.git
    GIT_TAG main
)
FetchContent_MakeAvailable(VulkanHeaders)

FetchContent_Declare(
    VulkanLoader
    GIT_REPOSITORY https://github.com/KhronosGroup/Vulkan-Loader.git
    GIT_TAG main
)
FetchContent_MakeAvailable(VulkanLoader)
