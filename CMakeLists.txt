cmake_minimum_required(VERSION 3.15)
project(AGI VERSION 0.1 LANGUAGES C CXX)

message(STATUS "AGI: Version ${PROJECT_VERSION}")

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(AGI_EXAMPLES "Build AGI-Examples and it's dependencies" ON)

add_compile_definitions(
    $<$<PLATFORM_ID:Windows>:AGI_WINDOWS>
    $<$<PLATFORM_ID:Linux>:AGI_LINUX>
    $<$<PLATFORM_ID:Darwin>:AGI_MACOSX>

    $<$<CONFIG:Debug>:AGI_DEBUG>
    $<$<CONFIG:Release>:AGI_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:AGI_RELEASE>
    $<$<CONFIG:MinSizeRel>:AGI_DIST>
)

add_subdirectory(src)
add_subdirectory(lib/glad)

if (AGI_EXAMPLES)
    add_subdirectory(examples)
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/icaven/glm.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(glm)