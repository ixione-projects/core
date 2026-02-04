cmake_minimum_required(VERSION 3.28)

project(ccore VERSION 0.0.0 LANGUAGES C CXX)
set(cmake_package_name CCore CACHE INTERNAL "CMake PackageName")
set(cmake_export_name "${cmake_package_name}Targets" CACHE INTERNAL "CMake ExportName")

option(BUILD_SHARED_LIBS "Build shared libraries (.so)" OFF)

set(cmake_install_generatorsdir "${CMAKE_BINARY_DIR}/generators" CACHE INTERNAL "generated sources install directory")
set(cmake_install_cmakedir "${CMAKE_INSTALL_LIBDIR}/cmake/${cmake_package_name}" CACHE INTERNAL "cmake install directory")
set(cmake_install_pkgconfigdir "${CMAKE_INSTALL_LIBDIR}/pkgconfig" CACHE INTERNAL "pkgconfig install directory")

set(config_file "${cmake_install_generatorsdir}/${cmake_package_name}Config.cmake" CACHE INTERNAL "<PackageName>Config.cmake file")
set(config_version_file "${cmake_install_generatorsdir}/${cmake_package_name}ConfigVersion.cmake" CACHE INTERNAL "<PackageName>ConfigVersion.cmake file")

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/${cmake_package_name}Config.cmake.in" ${config_file}
    INSTALL_DESTINATION ${cmake_install_cmakedir}
)
write_basic_package_version_file(${config_version_file} VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)

install(EXPORT ${cmake_export_name} DESTINATION ${cmake_install_cmakedir}
    NAMESPACE "${package_name}::"
    COMPONENT ${PROJECT_NAME}
)
install(FILES ${config_file} ${config_version_file} DESTINATION ${cmake_install_cmakedir}
    COMPONENT ${PROJECT_NAME}
)

set(pkgconfig_file "${cmake_install_generatorsdir}/${PROJECT_NAME}.pc" CACHE INTERNAL "<ProjectName>.pc file")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in" ${pkgconfig_file} @ONLY)
install(FILES ${pkgconfig_file} DESTINATION ${cmake_install_pkgconfigdir}
    COMPONENT ${PROJECT_NAME}
)

add_library(${PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/core.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/primitive/strings.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/primitive/strings/strings_reader.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/primitive/encodings/latin1.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/serialization/json/lexer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/serialization/json/parser.c"
)
add_library(${cmake_package_name}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_options(${PROJECT_NAME} PRIVATE ${cxx_default_compile_options} ${cxx_scrict_compile_options})
target_compile_features(${PROJECT_NAME} PUBLIC c_std_17 cxx_std_20)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/c/ccore>"
)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/include/c"
        FILES
            "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/core.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/strings.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/reader.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/strings/strings_reader.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/encodings.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/encodings/latin1.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/unicode.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/c/${PROJECT_NAME}/serialization/json.h"
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${cmake_export_name}
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(BUILD_GTEST)
    enable_testing()

    add_executable(${PROJECT_NAME}_test
        "${CMAKE_CURRENT_SOURCE_DIR}/src/c/serialization/json/lexer_test.cxx"
    )

    target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest_main ${cmake_package_name}::${PROJECT_NAME})
    target_compile_options(${PROJECT_NAME}_test PRIVATE ${cxx_default_compile_options})
    target_compile_features(${PROJECT_NAME}_test PRIVATE c_std_17 cxx_std_20)

    gtest_discover_tests(${PROJECT_NAME}_test)
endif()
