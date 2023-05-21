cmake_minimum_required(VERSION 3.25)
include(CMakePrintHelpers)

project(LEM)

add_subdirectory(lib/)
add_subdirectory(ut/ EXCLUDE_FROM_ALL)

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


