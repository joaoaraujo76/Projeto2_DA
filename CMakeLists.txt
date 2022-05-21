cmake_minimum_required(VERSION 3.20)
project(Projeto2)

set(CMAKE_CXX_STANDARD 14)

set(Headers include/Graph.h)
add_executable(Projeto2 main.cpp src/Graph.cpp)
