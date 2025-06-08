cmake_minimum_required(VERSION 3.15)
project(AGI VERSION 0.1 LANGUAGES C CXX)

message(STATUS "AGI: Version ${PROJECT_VERSION}")

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(AGI_EXAMPLES "Build AGI examples and it's dependencies" ON)

set(MSDF_ATLAS_USE_VCPKG OFF CACHE BOOL "" FORCE)
set(MSDFGEN_USE_SKIA OFF CACHE BOOL "" FORCE)

add_subdirectory(src)
add_subdirectory(agi-ext)

if (AGI_EXAMPLES)
    add_subdirectory(examples)
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/icaven/glm.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(glm)