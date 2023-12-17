#!/bin/bash

cmake -B build -D CMAKE_OSX_ARCHITECTURES="arm64;x86_64" -G Xcode
cmake --build build --config Debug
