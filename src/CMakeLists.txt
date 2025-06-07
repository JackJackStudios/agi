file(GLOB_RECURSE SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/**.cpp")
add_library(AGI STATIC ${SOURCE_DIR})

target_include_directories(AGI PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_precompile_headers(AGI PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include/agipch.hpp")

target_include_directories(AGI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_link_libraries(AGI PUBLIC glm glad glfw)

target_compile_definitions(AGI PUBLIC
    $<$<PLATFORM_ID:Windows>:AGI_WINDOWS>
    $<$<PLATFORM_ID:Linux>:AGI_LINUX>
    $<$<PLATFORM_ID:Darwin>:AGI_MACOSX>

    $<$<CONFIG:Debug>:AGI_DEBUG>
    $<$<CONFIG:Release>:AGI_RELEASE>
    $<$<CONFIG:RelWithDebInfo>:AGI_RELEASE>
    $<$<CONFIG:MinSizeRel>:AGI_DIST>

    AGI_VERSION="${PROJECT_VERSION}"
)