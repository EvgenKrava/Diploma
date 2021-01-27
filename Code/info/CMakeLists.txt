cmake_minimum_required(VERSION 3.17)
project(info)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_STATIC "-static-libgcc -static-libstdc++ -static")

add_executable(info main.cpp)