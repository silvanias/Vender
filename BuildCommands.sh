#!/bin/bash

# Change to the 'build' directory
cd build

# Build using CMake
cmake --build .

# Execute the 'vender' command in the 'build' directory
./vender
