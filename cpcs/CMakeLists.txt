cmake_minimum_required(VERSION 3.26)
project(cpcs C)

set(CMAKE_C_STANDARD 11)

find_library(JSON-C_LIBRARY json-c)

if (NOT JSON-C_LIBRARY)
    message(FATAL_ERROR "JSON-C library not found")
endif ()

add_executable(cpcs main.c COM.c
        sensor.h
        sensor.c
)

target_link_libraries(cpcs ${JSON-C_LIBRARY})