#include "sdk.h"


int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    if (argc != 2) {
        VTableError_File(TYPE_ERROR_FILE_NOT_FOUND,
            "INSUFFICENT ARGUMENTS",
            "NOT FILE THE PAST AN ARGUMENT");
        return EXIT_FAILURE;
    }

    char* content = NULL;
    size_t size = 0;

    Errno e = io_read_file(argv[1], &content, &size);
    if (e != 0) {
        fprintf(stderr, "could not read file: %s\n", strerror(e));
        return EXIT_FAILURE;
    }

    PE_FILE pe;
    bool is_pe = pe_parser_from_buffer(content, size, &pe);

    if (!is_pe) {
        free(content);
        return EXIT_FAILURE;
    }

    free(content);
    return EXIT_SUCCESS;
}