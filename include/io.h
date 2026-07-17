#ifndef IO_H
#define IO_H

#include "pch.h"
#include <errno.h>
#include <assert.h>

typedef int Errno;

Errno io_read_file(const char* path, char** content, size_t* size);

#ifdef IO_IMPLEMENTATION

Errno io_read_file(const char* path, char** content, size_t* size) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        return errno;
    }

    if (fseek(file, 0, SEEK_END) < 0) {
        int err = errno;
        fclose(file);
        return err;
    }

    long file_size = ftell(file);
    if (file_size < 0) {
        int err = errno;
        fclose(file);
        return err;
    }

    if (fseek(file, 0, SEEK_SET) < 0) {
        int err = errno;
        fclose(file);
        return err;
    }

    char* data = malloc((size_t)file_size);
    assert(data != NULL);

    if (fread(data, 1, (size_t)file_size, file) != (size_t)file_size) {
        int err = ferror(file) ? EIO : 0;
        free(data);
        fclose(file);
        return err;
    }

    fclose(file);

    *content = data;
    *size = (size_t)file_size;
    return 0;
}

#endif
#endif