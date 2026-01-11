cmake_minimum_required(VERSION 3.28)

project(core-distributions)

option(BUILD_C "Build the c-core subproject" OFF)
option(BUILD_LUA "Build the lua-core subproject" OFF)

option(BUILD_TEST "Build the test-suites" OFF)

if(BUILD_TESTS)
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

function(package_install_library cmake_package_name cmake_export_name library_name shared compile_options link_libraries)
    set(sources)
    set(header_dirs)
    set(headers)

    set(source_file_exts ".c;.cxx")
    set(header_file_exts ".h;.hxx")
    foreach(file ${ARGN})
        cmake_path(GET file EXTENSION ext)
        if(${ext} IN_LIST source_file_exts)
            list(APPEND sources ${file})
        elseif(${ext} IN_LIST header_file_exts)
            list(APPEND headers ${file})
            cmake_path(GET file PARENT_PATH dir)
            list(APPEND header_dirs ${dir})
        else()
            string(REPLACE ";" "|" source_file_exts "${source_file_exts}")
            string(REPLACE ";" "|" header_file_exts "${header_file_exts}")
            message(FATAL_ERROR 
                "\n package_install_library called with unexpected file \"${file}\", the recognized"
                "\n file extensions are: ${source_file_exts} or ${header_file_exts}"
            )
            return()
        endif()
    endforeach()
    list(REMOVE_DUPLICATES header_dirs)

    add_library(${library_name} ${shared} ${sources})
    add_library(${cmake_package_name}::${library_name} ALIAS ${library_name})
    if(link_libraries)
        target_link_libraries(${library_name} ${link_libraries})
    endif()

    set_target_properties(${library_name}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
    )
    target_compile_options(${library_name} PRIVATE ${compile_options})
    target_compile_features(${library_name} PUBLIC cxx_std_20)
    target_sources(${library_name}
        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ${header_dirs}
        FILES
            ${headers}
    )

    install(TARGETS ${library_name}
        EXPORT ${cmake_export_name}
        FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${library_name}"
        COMPONENT ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    get_target_property(interface_include_directories ${library_name} INTERFACE_INCLUDE_DIRECTORIES)
    list(APPEND interface_include_directories "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
    set_target_properties(${library_name}
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${interface_include_directories}"
    )
endfunction()

function(package_install_descriptors package_name)
    set(cmake_install_generatorsdir "${CMAKE_CURRENT_BINARY_DIR}/generators" CACHE INTERNAL "generated sources install directory")
    set(cmake_install_pkgconfigdir "${CMAKE_INSTALL_LIBDIR}/pkgconfig" CACHE INTERNAL "pkgconfig install directory")
    set(${package_name}_cmake_install_cmakedir "${CMAKE_INSTALL_LIBDIR}/cmake/${package_name}" CACHE INTERNAL "cmake install directory")

    set(${package_name}_config_file "${cmake_install_generatorsdir}/${package_name}Config.cmake" CACHE INTERNAL "<PackageName>Config.cmake file")
    set(${package_name}_config_version_file "${cmake_install_generatorsdir}/${package_name}ConfigVersion.cmake" CACHE INTERNAL "<PackageName>ConfigVersion.cmake file")

    configure_package_config_file("cmake/${package_name}Config.cmake.in" ${${package_name}_config_file} INSTALL_DESTINATION ${${package_name}_cmake_install_cmakedir})
    write_basic_package_version_file(${${package_name}_config_version_file} VERSION ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)

    install(EXPORT ${cmake_export_name} DESTINATION ${${package_name}_cmake_install_cmakedir}
        NAMESPACE "${package_name}::"
        COMPONENT ${PROJECT_NAME}
    )
    install(FILES ${${package_name}_config_file} ${${package_name}_config_version_file} DESTINATION ${${package_name}_cmake_install_cmakedir}
        COMPONENT ${PROJECT_NAME}
    )

    foreach(pkconfig_file_in ${ARGN})
        string(LENGTH ${pkconfig_file_in} length_in)
        math(EXPR length "${length_in} - 3")
        string(SUBSTRING ${pkconfig_file_in} 0 ${length} pkgconfig_filename)
        set(pkgconfig_file "${cmake_install_generatorsdir}/${pkgconfig_filename}" CACHE INTERNAL "<ProjectName>.pc file")

        configure_file("cmake/${pkconfig_file_in}" ${pkgconfig_file} @ONLY)
        install(FILES ${pkgconfig_file} DESTINATION ${cmake_install_pkgconfigdir}
            COMPONENT ${PROJECT_NAME}
        )
    endforeach()
endfunction()

function(package_gtest_main_executable cmake_package_name compile_options link_libraries)
    if(TARGET ${PROJECT_NAME}_test)
        message(FATAL_ERROR "Target ${PROJECT_NAME}_test is already defined.")
        return()
    endif()

    enable_testing()

    add_executable(${PROJECT_NAME}_test ${ARGN})
    target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest_main ${link_libraries})
    target_compile_options(${PROJECT_NAME}_test PRIVATE ${compile_options})
    target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_20)

    gtest_discover_tests(${PROJECT_NAME}_test)
endfunction()

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

if(NOT subproject_selected)
    message(FATAL_ERROR 
        "\n Must enable a least one subproject to build"
        "\n Run \"cmake -DBUILD_[subproject]=ON ...\""
        "\n     Where 'subproject' is one of:"
        "\n         C"
        "\n         LUA"
        "\n Example: cmake -DBUILD_C=ON ..."
    )
endif()

if(NOT TARGET uninstall)
    configure_file("${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
    add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()
