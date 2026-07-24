/*
    CG: gerador de código C (code generator).
    Traduz a AST produzida pelo parser (ast.h) para código-fonte C
    equivalente, escrevendo num FILE* já aberto pelo chamador.
    Header-only, no mesmo padrão de ast.h/token.h/io.h.
*/

#ifndef _CG_H
#define _CG_H

#include "pch.h"
#include "sv.h"
#include "ast.h"

/* Gera um arquivo .c completo e compilável a partir de "ast" (deve
   ser um nó AST_STATEMENTS, o resultado de
   parser_ast_from_tokens), escrevendo em "fp". O chamador é dono de
   "fp" -- abrir antes e fechar depois de chamar esta função. */
void cg_ast_to_c(FILE* fp, const AST* ast);

#ifdef CG_IMPLEMENTATION

/* forward declarations dos helpers internos, na ordem em que
   cg_ast_to_c vai descer até eles */
static void cg_write_main(FILE* fp, const AST* ast);
static void cg_write_statement(FILE* fp, const AST* ast, size_t indentation);
static void cg_write_function_call(FILE* fp, const AST* ast);
static void cg_write_expression(FILE* fp, const AST* ast);

/* escreve os #include necessários no topo do .c gerado */
static void
cg_write_includes(FILE* fp)
{
    fprintf(fp, "#include <stdio.h>\n");
    fprintf(fp, "\n");
}

/* escreve uma expressão (algo que produz um valor: string literal,
   ou o resultado de outra chamada de função) */
static void
cg_write_expression(FILE* fp, const AST* ast)
{
    switch (ast->kind) {
    case AST_STRING:
        fprintf(fp, "\"" SV_Fmt "\"", SV_Arg(ast->value.string));
        break;

    case AST_FUNCTION_CALL:
        cg_write_function_call(fp, ast);
        break;

    default:
        assert(0 && "expected an expression");
        break;
    }
}

/* escreve "nome(arg1, arg2, ...)", sem ';' no final -- quem chama
   decide se isso é um statement (bota ';') ou um argumento aninhado
   de outra chamada (não bota) */
static void
cg_write_function_call(FILE* fp, const AST* ast)
{
    AST_Value_Function_Call fn = ast->value.function_call;

    fprintf(fp, SV_Fmt, SV_Arg(fn.function));
    fprintf(fp, "(");
    for (size_t i = 0; i < fn.arguments.count; i++) {
        cg_write_expression(fp, fn.arguments.data[i]);
        if (i < fn.arguments.count - 1) {
            fprintf(fp, ", ");
        }
    }
    fprintf(fp, ")");
}

/* escreve um statement (um nó por linha), indentado com "\t" */
static void
cg_write_statement(FILE* fp, const AST* ast, size_t indentation)
{
    for (size_t i = 0; i < indentation; i++) {
        fprintf(fp, "\t");
    }

    switch (ast->kind) {
    case AST_FUNCTION_CALL:
        cg_write_function_call(fp, ast);
        fprintf(fp, ";\n");
        break;

    default:
        assert(0 && "expected a statement");
        break;
    }
}

/* escreve o int main(...) { ... } gerado, um statement por linha */
static void
cg_write_main(FILE* fp, const AST* ast)
{
    fprintf(fp, "int main(int argc, char *argv[]) {\n");
    for (size_t i = 0; i < ast->value.statements.count; i++) {
        cg_write_statement(fp, ast->value.statements.data[i], 1);
    }
    fprintf(fp, "\treturn 0;\n");
    fprintf(fp, "}\n");
}

/* ponto de entrada: chama includes e depois desce pra main */
void
cg_ast_to_c(FILE* fp, const AST* ast)
{
    cg_write_includes(fp);
    cg_write_main(fp, ast);
}

#endif /* CG_IMPLEMENTATION */
#endif /* _CG_H */