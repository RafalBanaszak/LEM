cmake_minimum_required(VERSION 3.25)
include(CMakePrintHelpers)

# Allow the user to specify the path to the cross compiler
# Template cross compiler configurations can be found in the toolchain/ directory.
# The toolchain file in most cases will need to be modified to point to the correct paths and set platform specific flags.
set(TOOLCHAIN_CONFIG_FILE_PATH "" CACHE PATH "Path to the compiler configuration file.")
if(TOOLCHAIN_CONFIG_FILE_PATH)
    set(CMAKE_TOOLCHAIN_FILE ${TOOLCHAIN_CONFIG_FILE_PATH})
endif()

project(LEM)

# lib/ contais the source code of the library
add_subdirectory(lib/)

# Only build the unit tests if we are not cross compiling
if(NOT CMAKE_CROSSCOMPILING)
    add_subdirectory(ut/ EXCLUDE_FROM_ALL)
endif()

# Helper target to run clang-format on all source files
file(GLOB_RECURSE ALL_SOURCE_FILES lib/*.[ch])
file(GLOB UT_SOURCE_FILES ut/*.cpp)
list(APPEND ALL_SOURCE_FILES ${UT_SOURCE_FILES})
add_custom_target(
    format
    COMMAND clang-format
    -i
    ${ALL_SOURCE_FILES}
)


