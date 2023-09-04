# Everything in the main project except for main.cpp
# This library will be used in main executable and unit tests
add_library(myproj_lib Vector3.cpp)
target_compile_features(myproj_lib PUBLIC cxx_std_20)
target_include_directories(myproj_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# The final product!
add_executable(myproj main.cpp)

# The library rng is designed to be used as an exported package,
# but this example just added the project folder using add_subdirectory()
# so that myproj can use it without install() and find_package() stuff.
target_link_libraries(myproj PRIVATE myproj_lib rng)
