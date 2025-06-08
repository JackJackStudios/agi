file(GLOB_RECURSE SOURCE_DIR "./**.c" "./**.h")
add_library(glad STATIC ${SOURCE_DIR})

target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")