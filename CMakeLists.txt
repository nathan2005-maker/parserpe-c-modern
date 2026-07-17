cmake_minimum_required(VERSION 3.16)
project(readpe LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES
    cmd/main.c
    src/pch.c
    src/VTableError.c
    src/pe_parser.c
    src/io.c
)

add_executable(readpe ${SOURCES})

target_include_directories(readpe PRIVATE include)

target_precompile_headers(readpe PRIVATE include/pch.h)