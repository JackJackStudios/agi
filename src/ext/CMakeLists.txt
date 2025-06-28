
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

add_library(agi_window STATIC "Window.cpp")
target_include_directories(agi_window PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
target_link_libraries(agi_window PUBLIC agi glfw)

add_library(agi_imgui STATIC "ImGuiLayer.cpp")
target_include_directories(agi_imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
target_link_libraries(agi_imgui PUBLIC agi ImGui glfw)