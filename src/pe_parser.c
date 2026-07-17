
#include "pch.h"
#include "pe_parser.h"
#include <string.h>

bool pe_parser_from_buffer(const char* buffer, size_t size, PE_FILE* out) {
    if (size < sizeof(IMAGE_DOS_HEADER)) {
        VTableError_File(TYPE_ERROR_FILE_INVALID,
            "FILE TOO SMALL TO CONTAIN A DOS HEADER",
            NULL);
        out->is_valid_pe = false;
        return false;
    }

    memcpy(&out->dos_header, buffer, sizeof(IMAGE_DOS_HEADER));

    if (out->dos_header.e_magic != IMAGE_DOS_SIGNATURE) {
        VTableError_File(TYPE_ERROR_FILE_INVALID,
            "FILE IS NOT A VALID DOS EXECUTABLE",
            NULL);
        out->is_valid_pe = false;
        return false;
    }

    out->is_valid_pe = true;
    return true;
}