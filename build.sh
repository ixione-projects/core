#!/usr/bin/env bash

cmake -DCMAKE_BUILD_TYPE=Debug -B ./cmake-build-debug/ -S ./ -G Ninja && ninja -C ./cmake-build-debug/ all
