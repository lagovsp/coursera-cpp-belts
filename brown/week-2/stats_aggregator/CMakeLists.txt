cmake_minimum_required(VERSION 3.21)
project(stats_aggregator)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(stats_aggregator main.cpp)
