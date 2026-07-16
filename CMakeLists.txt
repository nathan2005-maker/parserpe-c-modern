cmake_minimum_required(VERSION 3.16)
project(readpe LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

#List all source files default
set(SOURCES
    cmd/main.c
    src/pch.c
)

#executable main
add_executable(readpe ${SOURCES})

target_precompile_headers(readpe PRIVATE include/pch.h)