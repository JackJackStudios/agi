include(FetchContent)

function(add_example example_name)
add_executable(${example_name} "${example_name}.cpp")

target_compile_definitions(${example_name} PUBLIC EXECUTABLE_NAME="${example_name}")
target_link_libraries(${example_name} PRIVATE AGI spdlog::spdlog)
target_include_directories(${example_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${stb_SOURCE_DIR})
endfunction()

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
add_example(${EXAMPLE_TARGET})
endforeach()