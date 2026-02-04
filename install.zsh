#!/usr/bin/env zsh

zsh build.zsh && cmake --install "./build/${CMAKE_BUILD_TYPE:="Debug"}"
