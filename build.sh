#!/usr/bin/env bash

target="all"
if [[ $# -ne 0 ]]; then
    target="$1"
fi

cmake -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_BUILD_TYPE=Debug -B ./cmake-build-debug/ -S ./ -G Ninja && cmake --build ./cmake-build-debug/ --target $target
