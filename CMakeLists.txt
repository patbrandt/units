cmake_minimum_required(VERSION 2.6)

project(units)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(units_test test/test.cpp)
target_include_directories(units_test PRIVATE ${PROJECT_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})
target_link_libraries(units_test ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

enable_testing()
add_test(test_all units_test)
