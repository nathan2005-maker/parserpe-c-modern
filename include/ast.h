/*
	AST: representação da árvore sintática abstrata, que constroi ums lista a partir
	do consumo de tokens.
*/

ifndef _AST_H
#define _AST_H

#include "pch.h"
#include "sv.h"
#include "token.h"
#include "da.h"

typedef struct AST AST;

/*Nó*/
typedef enum {
	AST_STRING = 0,
	AST_EOF,
}AST_KIND;

typedef union {
    String_View string; /* usado por AST_STRING */
} AST_VALUE;


AST_VALUE ast_value_make_string(const String_View string);

struct AST {
    AST_KIND kind;
    AST_VALUE value;
};

/* Aloca (malloc) e inicializa um novo nó de AST com "kind" e
   "value". O chamador é dono do ponteiro devolvido. */
AST* ast_new(const AST_Kind kind, const AST_VALUE    value);

/* Tenta consumir uma expressão do início de "tokens", devolvendo o
   nó resultante em "*expression". Devolve true em caso de sucesso e
   false se "tokens" estiver vazio ou o token atual não iniciar uma
   expressão reconhecida (nesse caso "*expression" não é escrito). */
bool parser_parse_expression(Tokens* tokens, AST** expression);

/* Ponto de entrada do parser: constrói a AST completa a partir de
   "tokens" (recebido por valor -- a cópia local é que é consumida).
   Devolve NULL se nada pôde ser parseado. */
AST* parser_ast_from_tokens(Tokens tokens);

#ifdef AST_IMPLEMENTATION

AST_VALUE
ast_value_make_string(const String_View string)
{
    return (AST_VALUE) {
        .string = string,
    };
}

AST*
ast_new(const AST_KIND kind, const AST_VALUE value)
{
    AST* result = malloc(sizeof(AST));
    assert(result != NULL);

    result->kind = kind;
    result->value = value;

    return result;
}

bool
parser_parse_expression(Tokens* tokens, AST** expression)
{
    if (tokens->count == 0) {
        return false;
    }

    if (tokens->data[0].kind == TOKEN_STRING) {
        Token token_string = tokens_shift(tokens);
        *expression = ast_new(AST_STRING, ast_value_make_string(token_string.value.string));
        return true;
    }

    return false;
}

AST*
parser_ast_from_tokens(Tokens tokens)
{
    AST* expression = NULL;
    parser_parse_expression(&tokens, &expression);
    return expression;
}

#endif
#endif
