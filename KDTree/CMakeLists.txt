cmake_minimum_required(VERSION 3.28)
project(KDTree)

set(CMAKE_CXX_STANDARD 17)

add_executable(KDTree main.cpp
        src/kdtree.cpp
        src/kdtree.h
)
