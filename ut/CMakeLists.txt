cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
add_subdirectory(./cpputest/ EXCLUDE_FROM_ALL)

add_executable(ut_exec EXCLUDE_FROM_ALL
    lem_test.cpp
    lem_allocation_test.cpp
    lem_send_test.cpp
    lem_dispatcher_test.cpp
    lem_timer_test.cpp
    )

target_link_libraries(ut_exec PRIVATE lem_for_ut)
target_link_libraries(ut_exec PRIVATE
    CppUTest::CppUTest
    CppUTest::CppUTestExt)


add_custom_target(ut
    COMMAND ./ut_exec
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "********** Running Unit Tests **********"
    )

add_dependencies(ut ut_exec)

