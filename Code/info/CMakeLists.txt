cmake_minimum_required(VERSION 3.17)
project(info)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_STATIC "-static-libgcc -static-libstdc++ -static")

add_executable(info main.cpp props.cpp props.h simulating_annealing.cpp simulating_annealing.h genetratin_params.h cost.cpp cost.h)

find_library(PTHREAD_LIBRARY pthread)
find_package(Threads REQUIRED)
if (THREADS_HAVE_PTHREAD_ARG)
    set_property(TARGET info PROPERTY COMPILE_OPTIONS "-pthread")
    set_property(TARGET info PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif ()
if (CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(info "${CMAKE_THREAD_LIBS_INIT}")
endif ()