cmake_minimum_required(VERSION 3.28)

project(dsa VERSION 0.0.1 LANGUAGES C CXX)

find_package(GTest REQUIRED CONFIG)
include(GoogleTest) # gtest_discover_tests

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
        "./src/cxx/utils/stack.cpp"
        "./src/cxx/main.cpp"
)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx")

enable_testing()

add_executable("${PROJECT_NAME}_test")
target_link_libraries("${PROJECT_NAME}_test" PRIVATE GTest::gtest_main)
target_sources("${PROJECT_NAME}_test"
    PRIVATE
        "./src/cxx/utils/stack.cpp"
        "./src/cxx/utils/stack_test.cpp"
)
target_include_directories("${PROJECT_NAME}_test" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx")

gtest_discover_tests("${PROJECT_NAME}_test")
