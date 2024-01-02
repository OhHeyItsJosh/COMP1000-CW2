cmake_minimum_required(VERSION 3.5)

add_subdirectory("Tasks/04-srgui/")
add_subdirectory("Core/")

target_include_directories(srgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../Core/")
target_link_libraries(srgui PUBLIC core)
