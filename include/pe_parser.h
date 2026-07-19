/*
Analisador de arquivos PE (Portable Executable)

Inspecionar o cabeçalho do arquivo PE pu DLLs do Windows.

Validar o layout PE em disco ('M': 0x4A, 'Z': 0x5A).

*/

#ifndef PE_PARSER_H
#define PE_PARSER_H

#include <windows.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    IMAGE_DOS_HEADER dos_header;
    bool is_valid_pe;
} PE_FILE;

bool pe_parser_from_buffer(const char* buffer, size_t size, PE_FILE* out);

#endif