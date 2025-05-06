cmake_minimum_required(VERSION 3.15)
project(AGI VERSION 0.1 LANGUAGES CXX)

message(STATUS "AGI: Version ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
    $<$<CONFIG:Debug>:AGI_DEBUG>
    $<$<CONFIG:Release>:AGI_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:AGI_RELEASE>
    $<$<CONFIG:MinSizeRel>:AGI_DIST>
)

set(AGI_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib")

# Actual static library
file(GLOB_RECURSE SOURCE_DIR "src/**.cpp" "src/**.h" "include/**.h" "lib/glad/src/**.c")
add_library(AGI STATIC ${SOURCE_DIR})

target_precompile_headers(AGI PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/agipch.h")
set_source_files_properties("lib/glad/src/glad.c" PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_include_directories(AGI PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"

    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/glm"
)
