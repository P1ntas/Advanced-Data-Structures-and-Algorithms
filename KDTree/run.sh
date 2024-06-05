#!/bin/bash

# List of commands to run

commands=(
    "cmake CMakeLists.txt"
    "make"
    "./clean_directory.sh"
    "clear"
    "./KDTree"

)

# Loop through the list and run each command
for cmd in "${commands[@]}";
do
    echo "Running: $cmd"
    $cmd
done