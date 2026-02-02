#!/usr/bin/env zsh

zsh build.zsh && ./build/Debug/tools/generate-unicode-headers $@
