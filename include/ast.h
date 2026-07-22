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
} AST_Value;


#ifdef AST_IMPLEMENTATION


AST_Value
ast_value_make_string(const String_View string)
{
    return (AST_Value) {
        .string = string,
    };
}

AST*
ast_new(const AST_Kind kind, const AST_Value value)
{
    AST* result = malloc(sizeof(AST));
    assert(result != NULL);

    result->kind = kind;
    result->value = value;

    return result;
}

#endif
#endif
