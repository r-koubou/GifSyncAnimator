@echo off

cmake -B build -A x64 -G "Visual Studio 17 2022"
cmake --build build --config Release
