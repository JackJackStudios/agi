# Global interface for other backends
add_subdirectory(OpenGL/glad)

file(GLOB SOURCE_DIR "Utils.cpp" "Window.cpp")
file(GLOB OPENGL_SOURCE "OpenGL/**.cpp")

add_library(agi STATIC ${SOURCE_DIR} ${OPENGL_SOURCE})

target_include_directories(agi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(agi PUBLIC glm glad glfw)

target_include_directories(agi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_precompile_headers(agi PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include/agipch.hpp")

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

if (AGI_EXTENSIONS)
    add_subdirectory(ext)
endif()