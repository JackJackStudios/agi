
include(FetchContent)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb/
    GIT_TAG        master
)
FetchContent_MakeAvailable(stb)
FetchContent_GetProperties(stb)

file(GLOB_RECURSE IMGUI_DIR
    "imgui/imgui.cpp"
    "imgui/imgui_draw.cpp"
    "imgui/imgui_tables.cpp"
    "imgui/imgui_widgets.cpp"
    "imgui/imgui_demo.cpp"
    "imgui/backends/imgui_impl_opengl3.cpp"
    "imgui/backends/imgui_impl_glfw.cpp"
)

add_library(agi_imgui STATIC ${IMGUI_DIR} "ImGuiLayer.cpp")
target_include_directories(agi_imgui PUBLIC ${stb_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(agi_imgui PUBLIC agi)