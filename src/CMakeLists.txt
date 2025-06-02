file(GLOB_RECURSE SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/**.cpp")
add_library(AGI STATIC ${SOURCE_DIR})

target_include_directories(AGI PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_precompile_headers(AGI PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include/agipch.hpp")

target_include_directories(AGI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")

target_link_libraries(AGI PUBLIC glm)
target_link_libraries(AGI PRIVATE glad glfw )

if(${AGI_USE_TEXT})
target_link_libraries(msdf-atlas-gen::msdf-atlas-gen)
endif()