cmake_minimum_required(VERSION 3.12)

project(rng
    LANGUAGES CXX # This option makes cmake find working compiler for the specified languages
    VERSION 1.0.0 # This value will be used to generate xxx-config-version.cmake file
)

add_subdirectory(src)