cmake_minimum_required(VERSION 3.28)

project(core VERSION 0.0.1 LANGUAGES CXX)
set(cmake_package_name Core CACHE INTERNAL "CMake PackageName")
set(cmake_export_name "${cmake_package_name}Targets" CACHE INTERNAL "CMake ExportName")

option(BUILD_SHARED_LIBS "Build shared libraries (.so)" OFF)

find_package(GTest REQUIRED CONFIG)
include(GoogleTest) # gtest_discover_tests

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(cmake_install_generatorsdir "${CMAKE_CURRENT_BINARY_DIR}/generators" CACHE INTERNAL "generated sources install directory")
set(cmake_install_cmakedir "${CMAKE_INSTALL_LIBDIR}/cmake/${cmake_package_name}" CACHE INTERNAL "cmake install directory")
set(cmake_install_pkgconfigdir "${CMAKE_INSTALL_LIBDIR}/pkgconfig" CACHE INTERNAL "pkgconfig install directory")

set(config_file "${cmake_install_generatorsdir}/${cmake_package_name}Config.cmake" CACHE INTERNAL "<PackageName>Config.cmake file")
set(config_version_file "${cmake_install_generatorsdir}/${cmake_package_name}ConfigVersion.cmake" CACHE INTERNAL "<PackageName>ConfigVersion.cmake file")
set(pkgconfig_file "${cmake_install_generatorsdir}/${PROJECT_NAME}.pc" CACHE INTERNAL "<ProjectName>.pc file")

configure_package_config_file("cmake/${cmake_package_name}Config.cmake.in" ${config_file} INSTALL_DESTINATION ${cmake_install_cmakedir})
write_basic_package_version_file(${config_version_file} VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)
configure_file("cmake/${PROJECT_NAME}.pc.in" ${pkgconfig_file} @ONLY)

install(EXPORT ${cmake_export_name} DESTINATION  ${cmake_install_cmakedir}
    NAMESPACE "${cmake_package_name}::"
    COMPONENT ${PROJECT_NAME}
)

install(FILES ${config_file} ${config_version_file} DESTINATION ${cmake_install_cmakedir}
    COMPONENT ${PROJECT_NAME}
)

install(FILES ${pkgconfig_file} DESTINATION ${cmake_install_pkgconfigdir}
    COMPONENT ${PROJECT_NAME}
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(cxx_default_compile_options "-Wall;-Wshadow;-Wconversion;-Wundef;-fexceptions")
    set(cxx_scrict_compile_options "-W;-Wpointer-arith;-Wreturn-type;-Wcast-qual;-Wwrite-strings;-Wswitch;-Wunused-parameter;-Wcast-align;-Winline;-Wredundant-decls;-Wchar-subscripts")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(cxx_default_compile_options "-Wall;-Wshadow;-Wundef;-fexceptions")
    set(cxx_scrict_compile_options "-Wextra;-Wno-unused-parameter;-Wno-missing-field-initializers")
endif()

add_library(${PROJECT_NAME})
add_library("${cmake_package_name}::${cmake_package_name}" ALIAS ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
)

target_compile_options(${PROJECT_NAME} PRIVATE ${cxx_default_compile_options} ${cxx_scrict_compile_options})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_sources(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/stack.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/integral.cpp"
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT ${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${cmake_export_name}
    COMPONENT ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(NOT TARGET uninstall)
    configure_file("cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()

enable_testing()

add_executable("${PROJECT_NAME}_test")
target_link_libraries("${PROJECT_NAME}_test" PRIVATE GTest::gtest_main ${cmake_package_name}::${cmake_package_name})

target_compile_options("${PROJECT_NAME}_test" PRIVATE ${cxx_default_compile_options})
target_compile_features("${PROJECT_NAME}_test" PRIVATE cxx_std_20)
target_sources("${PROJECT_NAME}_test"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/stack_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/integral_test.cpp"
)

gtest_discover_tests("${PROJECT_NAME}_test")
