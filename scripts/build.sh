#!/bin/bash

# Max and Linux build script

# Step 0: Get the directory of the script
dir=$(dirname "$(readlink -f "$0")")

# Step 1: Configure the project with CMake
echo "Configuring the project with CMake..."
cmake -S "$dir"/../ -B "$dir"/Builds

# Check if the configuration step was successful
if [ $? -ne 0 ]; then
    echo "CMake configuration failed. Exiting."
    exit 1
fi

# Step 2: Build the project with CMake
echo "Building the project with CMake..."
cmake --build "$dir"/Builds

# Check if the build step was successful
if [ $? -ne 0 ]; then
    echo "CMake build failed. Exiting."
    exit 1
fi

echo "Build completed successfully."
