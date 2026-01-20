#!/usr/bin/env zsh

zsh build.sh && cmake --install "./build/${CMAKE_BUILD_TYPE:="Debug"}"
