/* Leitura de arquivo completo com buffer.
   Pequena função auxiliar para ler um arquivo inteiro para um buffer
   alocado no heap. Retornando um código de status errno.

   Devolve o resultado como um único String_View (ponteiro + tamanho
   empacotados) em vez de dois parâmetros de saída separados -- assim
   é impossível dessincronizar ponteiro e tamanho por engano.
*/

#ifndef _IO_H
#define _IO_H

#include "pch.h"
#include "sv.h"
#include <errno.h>

typedef int Errno;

/**
     Lê todo o conteúdo do arquivo em "path" para um buffer recém-alocado
     via malloc, e devolve uma view sobre ele em "*out".

     Em caso de sucesso, "out->data" aponta para o novo buffer (o
     chamador é o proprietário e deve liberá-lo com
     free((void*)out->data)) e "out->count" contém seu tamanho em
     bytes. Em caso de falha, "*out" fica zerado (data = NULL,
     count = 0) e o valor de retorno é um código errno diferente de
     zero que descreve o erro ocorrido.
 */
Errno io_read_file(const char* path, String_View* out);

#ifdef IO_IMPLEMENTATION

Errno
io_read_file(const char* path, String_View* out)
{
    *out = sv_make(NULL, 0);

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

    *out = sv_make(data, (size_t)file_size);
    return 0;
}

#endif
#endif /* _IO_H */