cmake_minimum_required(VERSION 3.28)

macro(tools_add_executable target)
    add_executable(${target} ${ARGN})

    target_compile_options(${target} PRIVATE ${cxx_default_compile_options})
    target_compile_features(${target} PRIVATE c_std_17 cxx_std_20)
endmacro()

project(tools VERSION 0.0.0 LANGUAGES C CXX)

tools_add_executable(generate-unicode-headers
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/generate-unicode-headers.c"
)
