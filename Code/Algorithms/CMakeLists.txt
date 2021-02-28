cmake_minimum_required(VERSION 3.17)
project(Algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(Algorithms main.cpp cost.cpp cost.h genetratin_params.h simulating_annealing.h simulating_annealing.cpp props.cpp props.h )

find_library(PTHREAD_LIBRARY pthread)
find_package(Threads REQUIRED)
if (THREADS_HAVE_PTHREAD_ARG)
    set_property(TARGET Algorithms PROPERTY COMPILE_OPTIONS "-pthread")
    set_property(TARGET Algorithms PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif ()
if (CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(Algorithms "${CMAKE_THREAD_LIBS_INIT}")
endif ()