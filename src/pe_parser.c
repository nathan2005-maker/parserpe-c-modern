#include "pch.h"
#include "sdk.h"
#include "pe_parser.h"
#include <string.h>

bool pe_parser_from_buffer(const String_View buffer, PE_FILE* out) {
    if (buffer.count < sizeof(IMAGE_DOS_HEADER)) {
        VTableError_File(TYPE_ERROR_FILE_INVALID,
            "FILE TOO SMALL TO CONTAIN A DOS HEADER",
            NULL);
        out->is_valid_pe = false;
        return false;
    }

    memcpy(&out->dos_header, buffer.data, sizeof(IMAGE_DOS_HEADER));

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

bool pe_parser_get_section(const String_View buffer, const PE_FILE* pe,
    const char* section_name, const uint8_t** out_data, size_t* out_size,
    uint64_t* out_virtual_address)
{
    if (!pe->is_valid_pe) {
        VTableError_File(TYPE_ERROR_FILE_INVALID, "PE INVALIDO", NULL);
        return false;
    }

    LONG e_lfanew = pe->dos_header.e_lfanew;
    if (e_lfanew < 0 || (size_t)e_lfanew + sizeof(IMAGE_NT_HEADERS) > buffer.count) {
        VTableError_File(TYPE_ERROR_FILE_INVALID, "NT HEADERS FORA DOS LIMITES DO ARQUIVO", NULL);
        return false;
    }

    const uint8_t* base = (const uint8_t*)buffer.data;
    const IMAGE_NT_HEADERS* nt = (const IMAGE_NT_HEADERS*)(base + e_lfanew);

    if (nt->Signature != IMAGE_NT_SIGNATURE) {
        VTableError_File(TYPE_ERROR_FILE_INVALID, "ASSINATURA PE INVALIDA", NULL);
        return false;
    }

    const IMAGE_SECTION_HEADER* sections = IMAGE_FIRST_SECTION(nt);
    WORD num_sections = nt->FileHeader.NumberOfSections;

    size_t name_len = strlen(section_name);
    if (name_len > IMAGE_SIZEOF_SHORT_NAME) {
        name_len = IMAGE_SIZEOF_SHORT_NAME;
    }

    for (WORD i = 0; i < num_sections; i++) {
        const IMAGE_SECTION_HEADER* section = &sections[i];

        if (memcmp(section->Name, section_name, name_len) == 0) {
            size_t offset = section->PointerToRawData;
            size_t size = section->SizeOfRawData;

            if (offset + size > buffer.count) {
                VTableError_File(TYPE_ERROR_LIMITE_BUFFER, "SECAO FORA DOS LIMITES DO ARQUIVO", NULL);
                return false;
            }

            *out_data = base + offset;
            *out_size = size;
            *out_virtual_address = nt->OptionalHeader.ImageBase + section->VirtualAddress;
            return true;
        }
    }

    VTableError_File(TYPE_ERROR_FILE_INVALID, "SECAO NAO ENCONTRADA", section_name);
    return false;
}