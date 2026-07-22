#include "pch.h"
#include "sdk.h"

int
main(int argc, char** argv)
{
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
    ast_dump(program, 0);

    free(tokens.data);
    free((void*)source.data);

    return 0;
}