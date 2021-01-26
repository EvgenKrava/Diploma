cmake_minimum_required(VERSION 3.17)
project(SBox)

set(CMAKE_STATIC "-static-libgcc -static-libstdc++ -static")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_STATIC}")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

add_executable(SBox main.cpp sbox.cpp)
find_library(PTHREAD_LIBRARY pthread)
find_package(Threads REQUIRED)
if (THREADS_HAVE_PTHREAD_ARG)
    set_property(TARGET SBox PROPERTY COMPILE_OPTIONS "-pthread")
    set_property(TARGET SBox PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif ()
if (CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(SBox "${CMAKE_THREAD_LIBS_INIT}")
endif ()
