/*
    AST: representação da árvore sintática abstrata, que constroi ums lista a partir
    do consumo de tokens.
*/

#ifndef _AST_H
#define _AST_H

#include "pch.h"
#include "sv.h"
#include "token.h"
#include "da.h"

typedef struct AST AST;

/*Nó*/
typedef enum {
    AST_STRING = 0,
    AST_FUNCTION_CALL,
    AST_STATEMENTS,
    AST_EOF,
}AST_KIND;

/* Array dinâmico de AST* (ponteiros para nós). Usado tanto para os
   argumentos de uma chamada de função quanto para a lista de
   statements do programa todo. Tem os campos que APPEND() (da.h)
   espera -- data/count/capacity. */
typedef struct {
    AST** data;
    size_t count;
    size_t capacity;
} ASTs;

/* Valor de um nó AST_FUNCTION_CALL: nome da função chamada e seus
   argumentos (cada um por sua vez uma AST, ex.: AST_STRING). */
typedef struct {
    String_View function;
    ASTs arguments;
} AST_Value_Function_Call;

typedef union {
    String_View string;                        /* usado por AST_STRING */
    AST_Value_Function_Call function_call;      /* usado por AST_FUNCTION_CALL */
    ASTs statements;                            /* usado por AST_STATEMENTS */
} AST_VALUE;


AST_VALUE ast_value_make_string(const String_View string);

/* Constrói o AST_VALUE de um nó AST_FUNCTION_CALL a partir do nome
   da função e da lista de argumentos já parseados. */
AST_VALUE ast_value_make_function_call(const String_View function, const ASTs arguments);

struct AST {
    AST_KIND kind;
    AST_VALUE value;
};

/* Aloca (malloc) e inicializa um novo nó de AST com "kind" e
   "value". O chamador é dono do ponteiro devolvido. */
AST* ast_new(const AST_KIND kind, const AST_VALUE value);

/* Tenta consumir uma expressão do início de "tokens", devolvendo o
   nó resultante em "*expression". Devolve true em caso de sucesso e
   false se "tokens" estiver vazio ou o token atual não iniciar uma
   expressão reconhecida (nesse caso "*expression" não é escrito). */
bool parser_parse_expression(Tokens* tokens, AST** expression);

/* Consome um statement completo do início de "tokens" (por enquanto,
   uma expressão sozinha, ex.: uma chamada de função). Se "tokens"
   estiver vazio devolve um nó AST_EOF. Se o token atual não iniciar
   nenhum statement reconhecido, imprime um erro com a localização e
   encerra o processo (exit(1)). O chamador é dono do ponteiro
   devolvido. */
AST* parser_parse_statement(Tokens* tokens);

/* Ponto de entrada do parser: constrói a AST completa a partir de
   "tokens" (recebido por valor -- a cópia local é que é consumida).
   Consome statement após statement até esgotar "tokens" (ou até
   encontrar TOKEN_EOF) e devolve um único nó AST_STATEMENTS
   contendo todos eles. */
AST* parser_ast_from_tokens(Tokens tokens);

/* Imprime "ast" em stdout de forma legível e recursiva (indentando
   filhos), útil para depuração. "indentation" é o nível inicial de
   indentação (normalmente 0). */
void ast_dump(const AST* ast, int indentation);

#ifdef AST_IMPLEMENTATION

AST_VALUE
ast_value_make_string(const String_View string)
{
    return (AST_VALUE) {
        .string = string,
    };
}

AST_VALUE
ast_value_make_function_call(const String_View function, const ASTs arguments)
{
    return (AST_VALUE) {
        .function_call = (AST_Value_Function_Call){
            .function = function,
            .arguments = arguments,
        },
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

    /* chamada de função: ID seguido de '(' argumentos... ')' */
    if (tokens->data[0].kind == TOKEN_ID &&
        tokens->count > 1 &&
        tokens->data[1].kind == TOKEN_LPAREN) {

        Token token_id = tokens_shift(tokens);
        tokens_shift(tokens); /* consome o '(' */

        ASTs arguments = { 0 };

        while (tokens->count > 0 && tokens->data[0].kind != TOKEN_RPAREN) {
            AST* argument = NULL;
            if (!parser_parse_expression(tokens, &argument)) {
                fprintf(stderr, Loc_Fmt " could not parse function call argument\n",
                    Loc_Arg(tokens->data[0].location));
                exit(1);
            }
            APPEND(&arguments, argument);
        }

        if (tokens->count == 0) {
            fprintf(stderr, "unexpected end of tokens: expected ')'\n");
            exit(1);
        }

        tokens_shift(tokens); /* consome o ')' */

        *expression = ast_new(AST_FUNCTION_CALL,
            ast_value_make_function_call(token_id.value.id, arguments));
        return true;
    }

    return false;
}

AST*
parser_parse_statement(Tokens* tokens)
{
    if (tokens->count == 0) {
        return ast_new(AST_EOF, (AST_VALUE) { 0 });
    }

    AST* expression = NULL;
    if (parser_parse_expression(tokens, &expression)) {
        return expression;
    }

    fprintf(stderr, Loc_Fmt " could not parse\n", Loc_Arg(tokens->data[0].location));
    exit(1);
}

AST*
parser_ast_from_tokens(Tokens tokens)
{
    AST* result = ast_new(AST_STATEMENTS, (AST_VALUE) { 0 });

    while (tokens.count > 0) {
        AST* statement = parser_parse_statement(&tokens);
        if (statement->kind == AST_EOF) {
            break;
        }
        APPEND(&result->value.statements, statement);
    }

    return result;
}

static void
ast_dump_indent(int indentation)
{
    for (int i = 0; i < indentation; i++) {
        fprintf(stdout, "  ");
    }
}

void
ast_dump(const AST* ast, int indentation)
{
    ast_dump_indent(indentation);

    switch (ast->kind) {
    case AST_STRING:
        fprintf(stdout, "STRING: " SV_Fmt "\n", SV_Arg(ast->value.string));
        break;

    case AST_FUNCTION_CALL:
        fprintf(stdout, "FUNCTION CALL: " SV_Fmt "\n", SV_Arg(ast->value.function_call.function));
        for (size_t i = 0; i < ast->value.function_call.arguments.count; i++) {
            ast_dump(ast->value.function_call.arguments.data[i], indentation + 1);
        }
        break;

    case AST_STATEMENTS:
        fprintf(stdout, "STATEMENTS:\n");
        for (size_t i = 0; i < ast->value.statements.count; i++) {
            ast_dump(ast->value.statements.data[i], indentation + 1);
        }
        break;

    case AST_EOF:
        fprintf(stdout, "EOF\n");
        break;
    }
}

#endif
#endif