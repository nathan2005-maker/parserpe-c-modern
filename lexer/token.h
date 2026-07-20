/* Token: tipos e lógica de tokenização (lexer) construída em cima
   do String_View (sv.h). Header-only, no mesmo padrão do io.h. */

#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "sv.h"
#include <stdio.h>

   /* Posição de um ponto no arquivo-fonte: caminho + linha + coluna.
      Existe só pra permitir mensagens de erro no formato
      "arquivo:linha:coluna: mensagem", igual compilador de verdade.
      row/column começam em 1 (convenção de editor), não em 0. */
typedef struct{
    const char* path;
    size_t row;
    size_t column;
} LOCATION;

LOCATION location_make(const char* path, const size_t row, const size_t column);


typedef struct
{
    String_View sv;
    Location location;
} Tokenizer;

Tokenizer tokenizer_make(const char* path, const String_View sv);

/*Categirua de token. */
typedef enum{
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ID,
    TOKEN_STRING,
    TOKEN_EOF,
} Token_Kind;

typedef union{
    String_View symbol; /* usado por TOKEN_LPAREN / TOKEN_RPAREN */
    String_View id;      /* usado por TOKEN_ID */
    String_View string;  /* usado por TOKEN_STRING (sem as aspas) */
} TOKEN_VALUE;

/*Um token extraído do arquivo - fonte: o que é(kind), o conteúdo
(value) e onde no arquivo ele começa(location, pra mensagens de
    erro do parser que vier depois). */
typedef struct{
    Token_Kind kind;
    Token_Value value;
    Location location;
} TOKEN;

TOKEN token_make(const Token_Kind kind, const Token_Value value, const Location location);

#ifdef TOKEN_IMPLEMENTATION

LOCATION
location_make(const char* path, const size_t row, const size_t column)
{
    return (LOCATION) {
        .path = path,
            .row = row,
            .column = column,
    };
}

Tokenizer
tokenizer_make(const char* path, const String_View sv)
{
    return (Tokenizer) {
        .sv = sv,
            .location = location_make(path, 1, 1),
    };
}

TOKEN
token_make(const Token_Kind kind, const Token_Value value, const Location location)
{
    return (Token) {
        .kind = kind,
            .value = value,
            .location = location,
    };
}

#endif /* TOKEN_IMPLEMENTATION */
#endif /* __TOKEN_H__ */