cmake_minimum_required(VERSION 3.12)

project(some_proj)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()
add_subdirectory(src)
add_subdirectory(test)