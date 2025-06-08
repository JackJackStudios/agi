# Global interface for other backends
file(GLOB SOURCE_DIR "Log.cpp" "RenderFactory.cpp" "Utils.cpp")
add_library(agi STATIC ${SOURCE_DIR})

target_include_directories(agi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(agi PUBLIC glm)

target_compile_definitions(agi PUBLIC
    $<$<PLATFORM_ID:Windows>:AGI_WINDOWS>
    $<$<PLATFORM_ID:Linux>:AGI_LINUX>
    $<$<PLATFORM_ID:Darwin>:AGI_MACOSX>

    $<$<CONFIG:Debug>:AGI_DEBUG>
    $<$<CONFIG:Release>:AGI_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:AGI_RELEASE>
    $<$<CONFIG:MinSizeRel>:AGI_DIST>

    AGI_VERSION="${PROJECT_VERSION}"
)

# OpenGL API
add_subdirectory(OpenGL/glad)

file(GLOB_RECURSE OPENGL_SOURCE "OpenGL/**.cpp")
add_library(agi_opengl STATIC ${OPENGL_SOURCE})

target_include_directories(agi_opengl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(agi_opengl PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include/agipch.hpp")

target_link_libraries(agi_opengl PRIVATE agi glad)