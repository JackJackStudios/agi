cmake_minimum_required(VERSION 3.15)
project(AGI VERSION 0.1 LANGUAGES C CXX)

message(STATUS "AGI: Version ${PROJECT_VERSION}")

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
    $<$<PLATFORM_ID:Windows>:AGI_WINDOWS>
    $<$<PLATFORM_ID:Linux>:AGI_LINUX>
    $<$<PLATFORM_ID:Darwin>:AGI_MACOSX>

    $<$<CONFIG:Debug>:AGI_DEBUG>
    $<$<CONFIG:Release>:AGI_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:AGI_RELEASE>
    $<$<CONFIG:MinSizeRel>:AGI_DIST>
)

add_subdirectory(lib/glad)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/icaven/glm.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(glm)

# Actual static library
file(GLOB_RECURSE SOURCE_DIR "src/**.cpp" "src/**.h" "include/**.h")
add_library(AGI STATIC ${SOURCE_DIR})

target_precompile_headers(AGI PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/agipch.h")

target_include_directories(AGI PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(AGI PUBLIC glm glad)