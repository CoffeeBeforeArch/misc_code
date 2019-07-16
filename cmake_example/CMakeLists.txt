# Set the minimum CMake version
cmake_minimum_required (VERSION 3.5)
# Name the project (sets PROJECT_NAME variable)
project (Powers)

# Add a version number
set (Powers_VERSION_MAJOR 1)
set (Powers_VERSION_MINOR 0)

# Configure a header file to pass CMake settings to the source
configure_file (
    "${PROJECT_SOURCE_DIR}/PowersConfig.h.in"
    "${PROJECT_BINARY_DIR}/PowersConfig.h"
    )

# Add the binary tree to the search path for include files
# This is necessary to find our generated header file
include_directories("${PROJECT_BINARY_DIR}")

# Builds an executable "Powers" from source "powers.cxx"
add_executable(Powers powers.cxx)
