#ifndef _SV_H
#define _SV_H

#include <stdlib.h>
#include <stdbool.h>


typedef struct {
	const char* data;
	size_t count;
} String_View;


#define SV_LITERAL(literal_) \
  (String_View) { .data = (literal_), .count = sizeof (literal_) - 1 }

/* atalho curto pro SV_LITERAL */
#define SV(cstr_lit) SV_LITERAL(cstr_lit)

   /*  printf/fprintf sem
	  depender de terminador nulo: "%.*s" recebe a precisão (tamanho)
	  como argumento antes do ponteiro.
	  Uso: fprintf(stdout, SV_Fmt "\n", SV_Arg(view)); */
#define SV_Fmt "%.*s"
#define SV_Arg(sv_) (int) (sv_).count, (sv_).data

	  /* função usada como critério em sv_chop_while */
typedef bool (*String_View_Predicate)(char c);

/* Constrói um String_View a partir de um ponteiro e tamanho já
   conhecidos em tempo de execução (ex.: um buffer lido de arquivo). */
String_View sv_make(const char* data, size_t count);

/* Compara dois String_View byte a byte. Diferente de strcmp(), não
   para no primeiro '\0' -- por isso é seguro para dados binários. */
bool sv_equals(const String_View a, const String_View b);

/* true se sv começa com os bytes de prefix */
bool sv_has_prefix(const String_View sv, const String_View prefix);

/* true enquanto c não for '"' */
bool sv_is_not_dquote(char c);

/* consome do início de sv enquanto predicate(byte) for true,
   avança sv e devolve o pedaço consumido */
String_View sv_chop_while(String_View* sv, String_View_Predicate predicate);

#endif /* _SV_H */