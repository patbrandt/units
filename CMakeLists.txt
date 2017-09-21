cmake_minimum_required(VERSION 2.6)

project(units)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

enable_testing()

find_package(GTest REQUIRED)
add_executable(units_test test/test.cpp)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(units_test ${GTEST_BOTH_LIBRARIES})

target_include_directories(units_test PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_test(test_all units_test)
