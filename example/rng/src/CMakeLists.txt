# Create a build target
add_library(${PROJECT_NAME} STATIC Random.cpp)

# Make an alias rng::rng so that we can maintain same usage for two cases:
# 1. find_package() to load a preinstalled library
# 2. place the library source inside the build tree and "add_subdirectory(rng)"
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Append 'd' to the .lib file to distinguish Debug/Release binaries
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Install rules for .lib and .h files
set(TargetExportName ${PROJECT_NAME}-targets)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${TargetExportName}
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)
install(FILES ${PROJECT_SOURCE_DIR}/include/rng/Random.h
    DESTINATION include/rng
)

# Generate rng-targets.cmake and rng-targets-<CONFIG>.cmake 
set(PackageCMakeDir lib/cmake/${PROJECT_NAME})
install(EXPORT ${TargetExportName}
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${PackageCMakeDir}
)

# Generate rng-config.cmake and rng-config-version.cmake
set(PackageConfig ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake)
set(PackageVersion ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake)

include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_SOURCE_DIR}/rng-config.cmake.in
    ${PackageConfig}
    INSTALL_DESTINATION ${PackageCMakeDir}
)
write_basic_package_version_file(${PackageVersion}
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(FILES ${PackageConfig} ${PackageVersion}
    DESTINATION ${PackageCMakeDir}
)