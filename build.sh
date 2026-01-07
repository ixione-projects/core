#!/usr/bin/env bash

build_type="Debug"
if [[ -n $1 ]]; then
    build_type=$1
fi

cmake -DCMAKE_FIND_DEBUG_MODE=ON -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_BUILD_TYPE=$build_type -B ./build/$build_type -S ./ -G Ninja && cmake --build ./build/$build_type --target all 
