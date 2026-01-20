#!/usr/bin/env zsh

cmake -DBUILD_GTEST=${BUILD_GTEST:="OFF"} -DBUILD_LUA=${BUILD_LUA:="OFF"} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX:="~/.local/"}" -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE:="Debug"}" -G "Ninja" -B "./build/${CMAKE_BUILD_TYPE}" -S "./" && cmake --build "./build/${CMAKE_BUILD_TYPE}" -- -j12
