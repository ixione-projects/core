cmake_minimum_required(VERSION 3.28)

project(luacore VERSION 0.0.0 LANGUAGES C CXX)
set(cmake_package_name LuaCore CACHE INTERNAL "CMake PackageName")
set(cmake_export_name "${cmake_package_name}Targets" CACHE INTERNAL "CMake ExportName")

find_package(CCore REQUIRED)
find_package(Lua REQUIRED)

add_library(Lua::Lua INTERFACE IMPORTED)
set_target_properties(Lua::Lua
    PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${LUA_LIBRARIES}"
)

if(NOT LUA)
    package_install_descriptors(${cmake_package_name} "${PROJECT_NAME}.pc.in")
endif()

set(cxx_compile_options ${cxx_default_compile_options} ${cxx_scrict_compile_options})
set(${PROJECT_NAME}_link_libraries PUBLIC CCore::ccore PRIVATE Lua::Lua)

if(PREFIX)
    set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
endif()

if(LUADIR)
    set(CMAKE_INSTALL_LUADIR ${LUADIR} CACHE INTERNAL "lua scripts install directory")
else()
    set(CMAKE_INSTALL_LUADIR "${CMAKE_INSTALL_PREFIX}/lua" CACHE INTERNAL "lua scripts install directory")
endif()

package_install_library(${cmake_package_name} ${cmake_export_name} ${PROJECT_NAME} SHARED "${cxx_compile_options}" "${${PROJECT_NAME}_link_libraries}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/c/sys.c"
)
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})

if(LUA)
    install(FILES 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/lua/core/core.lua" 
            DESTINATION "${CMAKE_INSTALL_LUADIR}/${cmake_package_name}"
        COMPONENT ${PROJECT_NAME}
    )
endif()
