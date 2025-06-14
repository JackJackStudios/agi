cmake_minimum_required(VERSION 3.15)
project(AGI VERSION 0.2 LANGUAGES C CXX)

message(STATUS "AGI: Version ${PROJECT_VERSION}")

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MSDF_ATLAS_USE_VCPKG OFF CACHE BOOL "" FORCE)
set(MSDFGEN_USE_SKIA OFF CACHE BOOL "" FORCE)

set(MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

option(AGI_EXAMPLES "Build AGI examples and it's dependencies" ${MAIN_PROJECT})
option(AGI_EXTENSIONS "Use agi-ext and download it's dependencies" ON)

add_subdirectory(src)

if (AGI_EXTENSIONS)
    add_subdirectory(agi-ext)
endif()

if (AGI_EXAMPLES)
    add_subdirectory(examples)
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/icaven/glm.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(glm)