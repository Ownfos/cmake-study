# Everything in the main project except for main.cpp
# This library will be used in main executable and unit tests
add_library(myproj_lib Vector3.cpp)
target_include_directories(myproj_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# The final product!
add_executable(myproj main.cpp)
target_link_libraries(myproj PRIVATE myproj_lib)
