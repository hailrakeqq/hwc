cmake_minimum_required(VERSION 3.26)
project(cpcs C)

set(CMAKE_C_STANDARD 11)

find_library(JSON-C_LIBRARY json-c)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UDEV REQUIRED libudev)

if (NOT JSON-C_LIBRARY)
    message(FATAL_ERROR "JSON-C library not found")
endif ()

include_directories(${UDEV_INCLUDE_DIRS})
link_directories(${UDEV_LIBRARY_DIRS})

add_executable(cpcs main.c COM.c
        sensor.h
        sensor.c
)

target_link_libraries(cpcs ${JSON-C_LIBRARY})
target_link_libraries(cpcs ${UDEV_LIBRARIES})