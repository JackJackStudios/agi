
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/JackKnox/imgui
    GIT_TAG docking
)
FetchContent_MakeAvailable(ImGui)

add_library(agi_imgui STATIC "ImGuiLayer.cpp")
target_include_directories(agi_imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
target_link_libraries(agi_imgui PUBLIC agi ImGui glfw)