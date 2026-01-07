#!/usr/bin/env zsh

cmake -G "Ninja" -DCMAKE_INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX:="~/.local/"}" -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE:="Debug"}" -B "./build/${CMAKE_BUILD_TYPE}" -S "./" && cmake --build "./build/${CMAKE_BUILD_TYPE}" -- -j12
