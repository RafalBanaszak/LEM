cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
add_subdirectory(./cpputest/ EXCLUDE_FROM_ALL)

add_executable(ut
    lem_test.cpp
    lem_allocation_test.cpp
    lem_send_test.cpp
    lem_dispatcher_test.cpp
    lem_timer_test.cpp
    )

target_link_libraries(ut PRIVATE lem_for_ut)
target_link_libraries(ut PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)
