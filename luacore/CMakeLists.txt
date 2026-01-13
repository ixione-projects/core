cmake_minimum_required(VERSION 3.28)

macro(package_install_lua_library library_pathname)
    package_install_library(
        ${cmake_package_name} ${cmake_export_name} "${library_pathname}" SHARED "${cxx_compile_options}" "${${PROJECT_NAME}_link_libraries}" ${ARGN}
    )

    string(REPLACE "/" "_" target ${library_pathname})
    if(LUA)
        set_target_properties(${target}
            PROPERTIES
                PREFIX ""
        )
    endif()
    target_include_directories(${target} PRIVATE ${LUA_INCLUDE_DIR})
    unset(target)
endmacro()

project(luacore VERSION 0.0.0 LANGUAGES C CXX)
set(cmake_package_name LuaCore CACHE INTERNAL "CMake PackageName")
set(cmake_export_name "${cmake_package_name}Targets" CACHE INTERNAL "CMake ExportName")

find_package(CCore REQUIRED)
find_package(Lua REQUIRED)

add_library(Lua::lua INTERFACE IMPORTED)
set_target_properties(Lua::lua
    PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}"
)

if(LUADIR)
    set(CMAKE_INSTALL_LUADIR ${LUADIR} CACHE INTERNAL "lua scripts install directory")
else()
    set(CMAKE_INSTALL_LUADIR "${CMAKE_INSTALL_PREFIX}/lua" CACHE INTERNAL "lua scripts install directory")
endif()

if(LUA)
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/lua/" DESTINATION ${CMAKE_INSTALL_LUADIR}
        COMPONENT ${PROJECT_NAME}
        FILES_MATCHING
            PATTERN "*.lua"
            PATTERN "*_test.lua" EXCLUDE
    )
else()
    package_install_descriptors(${cmake_package_name} "${PROJECT_NAME}.pc.in")
endif()

set(cxx_compile_options ${cxx_default_compile_options} ${cxx_scrict_compile_options})
set(${PROJECT_NAME}_link_libraries PUBLIC CCore::ccore PRIVATE Lua::lua)

package_install_lua_library("${PROJECT_NAME}/sys"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/sys.c"
)
