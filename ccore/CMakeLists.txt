cmake_minimum_required(VERSION 3.28)

project(ccore VERSION 0.0.0 LANGUAGES CXX)
set(cmake_package_name CCore CACHE INTERNAL "CMake PackageName")
set(cmake_export_name "${cmake_package_name}Targets" CACHE INTERNAL "CMake ExportName")

option(BUILD_SHARED_LIBS "Build shared libraries (.so)" OFF)

package_install_descriptors(${cmake_package_name} "${PROJECT_NAME}.pc.in")

set(cxx_compile_options ${cxx_default_compile_options} ${cxx_scrict_compile_options})
package_install_library(target ${cmake_package_name} ${cmake_export_name} ${PROJECT_NAME} "" "${cxx_compile_options}" ""
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/stringss.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/reader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/strings/strings_reader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/charsets.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/charsets/ascii.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/unicode.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/serialization/json.h"

    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/core.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/primitive/strings.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/primitive/strings/strings_reader.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/primitive/charsets/ascii.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/serialization/json/lexer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/serialization/json/parser.c"
)
target_include_directories(${target}
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/c>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>"
)

if(BUILD_GTEST)
    set(gtest_link_libraries ${cmake_package_name}::${PROJECT_NAME})
    package_gtest_main_executable(${PROJECT_NAME} "${cxx_default_compile_options}" "${gtest_link_libraries}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/c/serialization/json/lexer_test.cxx"
    )
endif()
