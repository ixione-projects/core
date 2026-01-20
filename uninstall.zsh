#!/usr/bin/env zsh

cmake --build "./build/${CMAKE_BUILD_TYPE:="Debug"}" --target uninstall
