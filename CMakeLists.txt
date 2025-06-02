cmake_minimum_required(VERSION 3.15)
project(AGI VERSION 0.1 LANGUAGES C CXX)

message(STATUS "AGI: Version ${PROJECT_VERSION}")

function(delcare_feature feature_name message default)
option(${feature_name} ${message} ${default})

if(${feature_name})
add_compile_definitions(${feature_name}=1)
endif()
endfunction()

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

delcare_feature(AGI_EXAMPLES "Build AGI-Examples and it's dependencies" ON)
delcare_feature(AGI_USE_WINDOW "Build AGI with window subsystem" ON)
delcare_feature(AGI_USE_TEXT "Build AGI with text rendering subsystem" OFF)

set(MSDF_ATLAS_USE_VCPKG OFF CACHE BOOL "" FORCE)
set(MSDFGEN_USE_SKIA OFF CACHE BOOL "" FORCE)

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

if (${AGI_USE_WINDOW})
    FetchContent_Declare(
        GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
    )
    FetchContent_MakeAvailable(GLFW)
endif()

if (${AGI_USE_TEXT})
FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype
    GIT_TAG        VER-2-13-3
)
FetchContent_MakeAvailable(freetype)
FetchContent_GetProperties(freetype)

set(FREETYPE_INCLUDE_DIRS "${freetype_SOURCE_DIR}/include")
set(FREETYPE_LIBRARY "${freetype_BINARY_DIR}/freetyped.lib")

FetchContent_Declare(
    msdf-atlas-gen
    GIT_REPOSITORY https://github.com/JackKnox/msdf-atlas-gen
    GIT_TAG        v1.2.2
)
FetchContent_MakeAvailable(msdf-atlas-gen)
endif()