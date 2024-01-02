cmake_minimum_required(VERSION 3.5)
project(core)

file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(core ${MY_SOURCES})
target_include_directories(core PUBLIC ${MY_HEADERS})
#target_sources(core)