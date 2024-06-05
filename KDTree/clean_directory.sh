#!/bin/bash

# List of files and directories to delete
files=(
    "./Makefile"
    "./cmake_install.cmake"
    "./CMakeFiles"
    "./CMakeCache.txt"
    "./KDTree"
)

# Loop through the list and delete each item
for item in "${files[@]}"; 
do
    if [ -e "$item" ]; then
        rm -rf "$item"
        echo "Deleted: $item"
    else
        echo "Not found: $item"
    fi
done
