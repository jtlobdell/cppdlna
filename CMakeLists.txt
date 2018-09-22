cmake_minimum_required (VERSION 3.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(core)
add_custom_target(core DEPENDS cppdlna)
