/*
    Disasm: um módulo escrito em C++ que usa a biblioteca Zydis para converter bytes de código de máquina em assembly x86-64 legível.
*/

#ifndef _DISASM_H
#define _DISASM_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* Decodifica e imprime (stdout), uma instrução por linha, os "size"
       bytes apontados por "code". "runtime_address" é o endereço que
       deve aparecer ao lado da primeira instrução (normalmente
       ImageBase + VirtualAddress da seção, o que pe_parser_get_section
       já devolve pronto) -- os endereços das instruções seguintes são
       calculados a partir dele, avançando o tamanho de cada instrução
       decodificada.

       Formato de cada linha: "<endereço em hex>  <mnemônico Intel>".
       Bytes que não formam uma instrução x86-64 válida são pulados um a
       um, imprimindo um marcador de erro nessa posição, pra não travar
       o disassembly inteiro por causa de dados/lixo no meio do código. */
    void disasm_print_bytes(FILE* fp, const uint8_t* code, size_t size, uint64_t runtime_address);

#ifdef __cplusplus
}
#endif

#endif /* _DISASM_H */