cmake_minimum_required(VERSION 3.15)
project(Keystone_Test2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Keystone_Test2 main.c)

if(APPLE)
    set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")
    set(CMAKE_C_COMPILER "/usr/local/bin/gcc")

    include_directories("/usr/local/include")
    target_link_libraries(Keystone_Test2 "/usr/local/lib/libkeystone.0.dylib")
endif(APPLE)