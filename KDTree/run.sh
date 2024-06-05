#!/bin/bash

# List of commands to run

commands=(
    "clear"
    "cmake CMakeLists.txt"
    "make"
    "./clean_directory.sh"
    #"clear"
    "./KDTree"

)

# Loop through the list and run each command
for cmd in "${commands[@]}";
do
    echo "Running: $cmd"
    $cmd

    # Check if the command was successful
    if [ $? -ne 0 ]; then
        echo "Error: $cmd failed"
        exit 1
    fi
done