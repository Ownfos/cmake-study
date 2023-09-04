# Prepare GoogleTest
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

# Add tests
function(add_test_executable file_name)
    # Create target
    add_executable(${file_name} ${file_name}.cpp)

    # Link main program and gtest
    target_link_libraries(${file_name} PRIVATE
        myproj_lib
        GTest::gtest_main
    )

    # Register tests
    gtest_discover_tests(${file_name})
endfunction()

add_test_executable(test1)
add_test_executable(test2)