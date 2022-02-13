cmake_minimum_required(VERSION 3.21)
project(week_5)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(task matrix_sum.cpp)
