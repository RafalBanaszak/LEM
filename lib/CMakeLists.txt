cmake_minimum_required(VERSION 3.25)

set(CMAKE_C_STANDARD 11)

# Define the ARCH option with possible values to include platform specific code
set(ARCH_VALUES "ARMV6M" "ARMV7M" "ARMV8M" "GENERIC")
if(CMAKE_CROSSCOMPILING)
    set(ARCH "unknown" CACHE STRING "Target architecture chosen by the user at CMake configure time")
else()
    set(ARCH "GENERIC" CACHE STRING "Target architecture chosen by the user at CMake configure time")
endif()
set_property(CACHE ARCH PROPERTY STRINGS ${ARCH_VALUES})

# Check if the selected architecture is valid
list(FIND ARCH_VALUES ${ARCH} ARCH_INDEX)
if(ARCH STREQUAL "unknown")
    message(FATAL_ERROR "Architecture must be selected.\n-DARCH=<value>.\nPossible values: ${ARCH_VALUES}")
elseif(ARCH_INDEX EQUAL -1)
    message(FATAL_ERROR "Invalid architecture selected. Please choose one of: ${ARCH_VALUES}")
endif()
message(STATUS "Selected architecture: ${ARCH}")

set(LIB_SOURCE_FILES
    src/lem.c
    src/internal/lem_dispatcher.c
    src/internal/lem_event.c
    src/internal/lem_queue.c
    src/internal/lem_soft_timer.c
    )

add_library(lem STATIC ${LIB_SOURCE_FILES})
target_include_directories(lem PUBLIC inc/)
target_compile_definitions(lem PUBLIC LEM_PLATFORM=LEM_PLATFORM_${ARCH})
target_compile_options(lem PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wdouble-promotion -Wundef -Wconversion -Wsign-conversion)

if(NOT CMAKE_CROSSCOMPILING)
    add_library(lem_for_ut STATIC EXCLUDE_FROM_ALL ${LIB_SOURCE_FILES} )
    target_include_directories(lem_for_ut PUBLIC inc/)
    target_compile_definitions(lem_for_ut PUBLIC LEM_PLATFORM=LEM_PLATFORM_GENERIC LEM_UT)
endif()

add_compile_options(
    "-Wall" "-Wextra"
    "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
    "$<$<CONFIG:RELEASE>:-O3>"
    )