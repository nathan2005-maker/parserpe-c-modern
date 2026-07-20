#ifndef _SV_H
#define _SV_H

#include <stdlib.h>
#include <stdbool.h>


typedef struct{
	const char* data;
	size_t count;
} String_View;


#define SV_LITERAL(literal_) \
  (String_View) { .data = (literal_), .count = sizeof (literal_) - 1 }

   /*  printf/fprintf sem
	  depender de terminador nulo: "%.*s" recebe a precisão (tamanho)
	  como argumento antes do ponteiro.
	  Uso: fprintf(stdout, SV_Fmt "\n", SV_Arg(view)); */
#define SV_Fmt "%.*s"
#define SV_Arg(sv_) (int) (sv_).count, (sv_).data

	  /* Constrói um String_View a partir de um ponteiro e tamanho já
		 conhecidos em tempo de execução (ex.: um buffer lido de arquivo). */
String_View sv_make(const char* data, size_t count);

/* Compara dois String_View byte a byte. Diferente de strcmp(), não
   para no primeiro '\0' -- por isso é seguro para dados binários. */
bool sv_equals(const String_View a, const String_View b);

#endif /* _SV_H */