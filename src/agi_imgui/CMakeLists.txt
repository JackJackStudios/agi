file(GLOB_RECURSE SOURCE_DIR "./**.cpp")
add_library(agi_imgui STATIC ${SOURCE_DIR})

target_include_directories(agi_imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(agi_imgui PUBLIC agi)