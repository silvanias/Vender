#!/bin/bash

# Change to the 'build' directory
cd build

# Build using CMake
cmake --build .

# Go back to the parent directory
cd ..

# Execute the 'vender' command in the 'build' directory
./build/vender
