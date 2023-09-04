cmake_minimum_required(VERSION 3.12)

project(some_proj)

enable_testing()
add_subdirectory(rng)
add_subdirectory(src)
add_subdirectory(test)