cmake_minimum_required(VERSION 3.21)
project(search_server)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(search_server main.cpp)
