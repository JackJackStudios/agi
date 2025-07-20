file(GLOB_RECURSE GLAD_DIR "./**.c")
add_library(glad STATIC ${GLAD_DIR})

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})