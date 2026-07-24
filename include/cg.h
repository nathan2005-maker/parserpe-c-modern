#ifndef CG_H
#define CG_H

/*
     "Code generator" que transforma a AST de volta em C de verdade, escreve num arquivo (a.c), compila esse arquivo com gcc e roda
*/

#include "pch.h"
#include "ast.h"
#include "sv.h"

/* Gera um arquivo .c completo e compilável a partir de "ast" (deve
   ser um nó AST_STATEMENTS, o resultado de
   parser_ast_from_tokens), escrevendo em "fp". O chamador é dono de
   "fp" -- abrir antes e fechar depois de chamar esta função. */
void cg_ast_to_c(FILE* fp, const AST* ast);

#endif
#ifdef CG_IMPLEMENTATION

/* forward declarations dos helpers internos, na ordem em que
   cg_ast_to_c vai descer até eles -- assim cg_ast_to_c já pode ser
   escrita agora, mesmo com os corpos vindo só nas próximas partes */
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

/* ponto de entrada: chama includes e depois desce pra main */
void
cg_ast_to_c(FILE* fp, const AST* ast)
{
    cg_write_includes(fp);
    cg_write_main(fp, ast);
}



#endif // CG_H