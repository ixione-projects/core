cmake_minimum_required(VERSION 3.28)

macro(tools_add_executable target)
    add_executable(${target} ${ARGN})

    target_compile_options(${target} PRIVATE ${cxx_default_compile_options})
    target_compile_features(${target} PRIVATE c_std_17 cxx_std_20)

    target_link_libraries(${target} PRIVATE CURL::libcurl tree-sitter)
endmacro()

project(tools VERSION 0.0.0 LANGUAGES C CXX)

find_package(CURL REQUIRED)
find_library(tree-sitter tree-sitter REQUIRED)

tools_add_executable(generate-unicode-headers
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/generate-unicode-headers.c"
)
