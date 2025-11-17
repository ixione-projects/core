cmake_minimum_required(VERSION 3.28)

project(dsa VERSION 0.0.1 LANGUAGES CXX)

find_package(GTest REQUIRED CONFIG)
include(GoogleTest) # gtest_discover_tests

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(dsa)
target_sources(dsa
    PUBLIC
        "./src/cxx/util/stack.hpp"
    PRIVATE
        "./src/cxx/main.cpp"
)

enable_testing()

add_executable(dsa_test)
target_link_libraries(dsa_test PRIVATE GTest::gtest_main)
target_sources(dsa_test
    PUBLIC
        "./src/cxx/util/stack.hpp"
    PRIVATE
        "./src/cxx/util/stack_test.cpp"
)

gtest_discover_tests(dsa_test)
