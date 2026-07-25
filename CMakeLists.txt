cmake_minimum_required(VERSION 3.16)
project(readpe LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(Dependencies.cmake)

set(SOURCES
    cmd/main.c
    src/pch.c
    src/VTableError.c
    src/pe_parser.c
    src/io.c
    src/sv.c
    src/ast.c
    src/token.c
    src/cg.c
    src/disasm.cpp
)

add_executable(readpe ${SOURCES})

target_include_directories(readpe PRIVATE include lexer)

target_link_libraries(readpe PRIVATE Zydis)

target_precompile_headers(readpe PRIVATE include/pch.h)