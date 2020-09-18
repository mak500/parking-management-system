#!/bin/bash

cmake -S . -B build
cd build
make clang-format
make clang-tidy
make doxygen
make
cd ..
