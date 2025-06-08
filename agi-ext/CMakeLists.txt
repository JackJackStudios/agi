
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/JackKnox/imgui
    GIT_TAG docking
)
FetchContent_MakeAvailable(ImGui)

FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(GLFW)

file(GLOB_RECURSE EXT_SOURCE "AGI/**.cpp")
add_library(agi_ext STATIC ${EXT_SOURCE})

target_include_directories(agi_ext PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(agi_ext PUBLIC agi glfw ImGui)