cmake_minimum_required(VERSION 3.28)

project(core-distributions)

option(BUILD_C "Build the c-core subproject" ON)
option(BUILD_LUA "Build the lua-core subproject" OFF)
option(BUILD_TOOLS "Build the tools subproject" OFF)

option(BUILD_GTEST "Build the gtest test-suites for the associated subproject(s)" OFF)

if(BUILD_GTEST)
    find_package(GTest REQUIRED)
    include(GoogleTest) # gtest_discover_tests
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(cxx_default_compile_options "-Wall;-Wshadow;-Wconversion;-Wundef;-fexceptions")
    set(cxx_scrict_compile_options "-W;-Wpointer-arith;-Wreturn-type;-Wcast-qual;-Wwrite-strings;-Wswitch;-Wunused-parameter;-Wcast-align;-Winline;-Wredundant-decls;-Wchar-subscripts")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(cxx_default_compile_options "-Wall;-Wshadow;-Wundef;-fexceptions")
    set(cxx_scrict_compile_options "-Wextra;-Wno-unused-parameter;-Wno-missing-field-initializers")
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

enable_testing()

set(subproject_selected OFF)

if(BUILD_C)
    set(subproject_selected ON)
    add_subdirectory(ccore)
endif()

if(BUILD_LUA)
    set(subproject_selected ON)
    add_subdirectory(luacore)
endif()

if(BUILD_TOOLS)
    set(subproject_selected ON)
    add_subdirectory(tools)
endif()

if(NOT subproject_selected)
    message(FATAL_ERROR
        "\n Must enable a least one subproject to build"
        "\n Run \"cmake -DBUILD_[subproject]=ON ...\""
        "\n     Where 'subproject' is one of:"
        "\n         C"
        "\n         LUA"
        "\n         TOOLS"
        "\n Example: cmake -DBUILD_C=ON ..."
    )
endif()

if(NOT TARGET uninstall)
    configure_file("${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()
