/*
    Leitura de arquivo completo com buffer.
    Pequena função auxiliar para ler um arquivo inteiro para um buffer
    alocado no heap. Retornando um código de status errno.
*/

#ifndef IO_H
#define IO_H

#include "pch.h"
#include <errno.h>
#include <assert.h>

typedef int Errno;

/**
     Lê todo o conteúdo do arquivo em "path" para um buffer recém - alocado
     via malloc.

     Em caso de sucesso, "*content" aponta para o novo buffer (o chamador
     é o proprietário e deve liberá-lo com free()) e "*size" contém seu
     tamanho em bytes. Em caso de falha, "*content" e "*size" permanecem
     inalterados e o valor de retorno é um código errno diferente de zero
     que descreve o erro ocorrido.

 */
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

    char* data = NULL;
    if (file_size > 0) {
        data = malloc((size_t)file_size);
        if (data == NULL) {
            fclose(file);
            return ENOMEM;
        }
    }

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