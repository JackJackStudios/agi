cmake_minimum_required(VERSION 3.15)
project(AGI-Examples)

include(FetchContent)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(GLFW)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(spdlog)

file(GLOB_RECURSE SOURCE_DIR "**.cpp")
add_executable(AGI-Examples ${SOURCE_DIR})

target_link_libraries(AGI-Examples PRIVATE AGI glfw spdlog::spdlog)
target_include_directories(AGI-Examples PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})