cmake_minimum_required(VERSION 3.28)

project(ccore VERSION 0.0.0 LANGUAGES CXX)
set(cmake_package_name CCore CACHE INTERNAL "CMake PackageName")
set(cmake_export_name "${cmake_package_name}Targets" CACHE INTERNAL "CMake ExportName")

option(BUILD_SHARED_LIBS "Build shared libraries (.so)" OFF)

package_install_descriptors(${cmake_package_name} "${PROJECT_NAME}.pc.in")

set(cxx_compile_options ${cxx_default_compile_options} ${cxx_scrict_compile_options})
package_install_library(${cmake_package_name} ${cmake_export_name} ${PROJECT_NAME} "" "${cxx_compile_options}" ""
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/stack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/integral.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cxx/${PROJECT_NAME}/stack.hxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cxx/${PROJECT_NAME}/integral.hxx"

    "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/stack.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/integral.cxx"
)

if(BUILD_TESTS)
    set(gtest_link_libraries ${cmake_package_name}::${PROJECT_NAME})
    package_gtest_main_executable(${cmake_package_name} "${cxx_default_compile_options}" "${gtest_link_libraries}" 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/stack_test.cxx"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/integral_test.cxx"
    )
endif()
