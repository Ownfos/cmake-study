# Make unit test targets
add_executable(test1 test1.cpp)
add_executable(test2 test2.cpp)

# Link libraries
find_package(GTest CONFIG REQUIRED)
target_link_libraries(test1 PRIVATE
    myproj_lib
    GTest::gtest_main
)
target_link_libraries(test2 PRIVATE
    myproj_lib
    GTest::gtest_main
)

# Register unit tests
include(GoogleTest)
gtest_discover_tests(test1)
gtest_discover_tests(test2)