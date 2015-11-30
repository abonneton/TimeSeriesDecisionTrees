# CMake configuration
project (decision_trees)
cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
find_package(Threads REQUIRED)

# setup compilation flags
set (warnings "-Wall -Wextra -pedantic -Werror")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} -std=c++11")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
add_subdirectory(src)
add_subdirectory(tools)
