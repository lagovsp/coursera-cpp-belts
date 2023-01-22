cmake_minimum_required(VERSION 3.21)
project(week_1)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(week_1 secondary_index.cpp)
