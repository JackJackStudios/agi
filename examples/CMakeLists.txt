include(FetchContent)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb/
    GIT_TAG        master
)
FetchContent_MakeAvailable(stb)
FetchContent_GetProperties(stb)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(spdlog)

file(GLOB_RECURSE EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/**.cpp") 

foreach(EXAMPLE ${EXAMPLES})
get_filename_component(EXAMPLE_TARGET ${EXAMPLE} NAME_WE)
add_executable(${EXAMPLE_TARGET} "${EXAMPLE_TARGET}.cpp")

target_compile_definitions(${EXAMPLE_TARGET} PUBLIC EXECUTABLE_NAME="${EXAMPLE_TARGET}")
target_link_libraries(${EXAMPLE_TARGET} PRIVATE agi agi_imgui agi_window agi_opengl spdlog::spdlog)
target_include_directories(${EXAMPLE_TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${stb_SOURCE_DIR})
endforeach()