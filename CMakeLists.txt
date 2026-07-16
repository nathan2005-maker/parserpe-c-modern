cmake_minimum_required(VERSION 3.16)
project(readpe LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    cmd/main.c
    src/pch.c
    src/VTableError.c
)

add_executable(readpe ${SOURCES})

target_include_directories(readpe PRIVATE include)

target_precompile_headers(readpe PRIVATE include/pch.h)
