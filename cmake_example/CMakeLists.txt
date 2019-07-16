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

# See if we should use our "myPow" function
option (USE_MYMATH "Use our own exponent function" ON)

# Act conditionally based on this option
if(USE_MYMATH)
    # Add directories for our "myPow" prototype
    include_directories ("${PROJECT_SOURCE_DIR}/local_functions")
    # Add subdirectory so that the "myPow" function will be built
    add_subdirectory (local_functions)
    set (EXTRA_LIBS ${EXTRA_LIBS} Exponent)
endif(USE_MYMATH)

# Builds an executable "Powers" from source "powers.cxx"
add_executable(Powers powers.cpp)
# Target our library for linking
target_link_libraries(Powers ${EXTRA_LIBS})

