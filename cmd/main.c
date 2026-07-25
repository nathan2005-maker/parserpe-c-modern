#include "pch.h"
#include "sdk.h"
#include "disasm.h"

int
main(int argc, char** argv)
{
    (void)argc;
    (void)argv[0];

    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    const char* path = argv[1];

    String_View source;
    Errno err = io_read_file(path, &source);
    if (err != 0) {
        VTableError_File(TYPE_ERROR_FILE_NOT_FOUND, "could not read source file", path);
        return 1;
    }

    Tokenizer tokenizer = tokenizer_make(path, source);
    Tokens tokens = tokenizer_tokenize(&tokenizer);

    AST* program = parser_ast_from_tokens(tokens);
    /* ast_dump(program, 0); */ /* descomente pra ver a AST no stdout */

    FILE* fp = fopen("a.c", "wb");
    if (fp == NULL) {
        fprintf(stderr, "could not open file: %s\n", strerror(errno));
        free(tokens.data);
        free((void*)source.data);
        return 1;
    }

    cg_ast_to_c(fp, program);
    fclose(fp);

    free(tokens.data);
    free((void*)source.data);

    /* compila o a.c gerado pra um executável, pra podermos
       desmontar o código de máquina resultante */
    int compile_status = system("gcc --std=c17 -o a.exe a.c");
    if (compile_status != 0) {
        fprintf(stderr, "falha ao compilar a.c\n");
        return 1;
    }

    /* lê o a.exe recém-compilado, acha a seção .text e imprime o
       assembly correspondente */
    String_View exe_buffer;
    err = io_read_file("a.exe", &exe_buffer);
    if (err != 0) {
        VTableError_File(TYPE_ERROR_FILE_NOT_FOUND, "could not read a.exe", "a.exe");
        return 1;
    }

    PE_FILE pe;
    if (!pe_parser_from_buffer(exe_buffer, &pe)) {
        free((void*)exe_buffer.data);
        return 1;
    }

    const uint8_t* code = NULL;
    size_t code_size = 0;
    uint64_t code_address = 0;

    if (pe_parser_get_section(exe_buffer, &pe, ".text", &code, &code_size, &code_address)) {
        puts("DISASSEMBLY (.text)");
        disasm_print_bytes(stdout, code, code_size, code_address);

        FILE* asm_fp = fopen("a.asm", "w");
        if (asm_fp == NULL) {
            fprintf(stderr, "could not open a.asm: %s\n", strerror(errno));
        }
        else {
            disasm_print_bytes(asm_fp, code, code_size, code_address);
            fclose(asm_fp);
            puts("disassembly saved to a.asm");
        }
    }

    free((void*)exe_buffer.data);

    return 0;
}