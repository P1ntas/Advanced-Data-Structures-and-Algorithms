cmake_minimum_required(VERSION 3.28)
project(KDTree)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(KDTree main.cpp
        src/kdtree.cpp
        src/kdtree.h
        src/song.h
        src/song.cpp
)
