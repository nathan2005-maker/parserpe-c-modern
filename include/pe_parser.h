/*
Analisador de arquivos PE (Portable Executable)

Inspecionar o cabeçalho do arquivo PE pu DLLs do Windows.

Validar o layout PE em disco ('M': 0x4A, 'Z': 0x5A).

*/

#ifndef _PE_PARSER_H
#define _PE_PARSER_H

#include <windows.h>
#include <stdbool.h>
#include "sv.h"

typedef struct {
    IMAGE_DOS_HEADER dos_header;
    bool is_valid_pe;
} PE_FILE;

/* Analisa "buffer" (uma view sobre os bytes brutos do arquivo, ex.:
   o resultado de io_read_file) como um arquivo PE, preenchendo
   "out". Retorna true se o buffer contém pelo menos um cabeçalho
   DOS válido. */
bool pe_parser_from_buffer(const String_View buffer, PE_FILE* out);

#endif /* _PE_PARSER_H */