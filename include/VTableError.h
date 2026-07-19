/*
	Relatório de erros.
	Um tipo de erro compacto, baseado em *tagged-union*, utilizado em toda a ferramenta

*/

#ifndef VTABLE_ERROR_H
#define VTABLE_ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* 
	Categoria do erro sendo relatado. Determina qual membro da
	união dentro de VTableError é válido para uma determinada instância. 
*/

typedef enum {
	TYPE_ERROR_FILE_NOT_FOUND = 1, //Error related to file operations
	TYPE_ERROR_FILE_INVALID,       //Error related to user input or actions
	TYPE_ERROR_LIMITE_BUFFER       //Error related to generation logs
}typeError;

typedef struct {
	typeError type;
	const char* message;

	union {
		struct {
			const char* name_file;
			uint8_t* buffer_error;
		}system_erro;

		struct {
			uint32_t limit_buffer;
			uint32_t found_buffer;
			const char* head_buffer;
		}limit_buffer_erro;
	};

}VTableError;

/* 
	Imprime uma mensagem de diagnóstico para "error" na stderr, formatada de acordo com
	error->type. Encerra o processo se "error" for NULL. 
*/
void VTableError_Init(VTableError* error, typeError type, const char* message);

/*
	Wrapper de conveniência para erros relacionados a arquivos : constrói um VTableError
	com o ramo "system_erro" preenchido a partir de "file_name" e
	o reporta via VTableError_Init(). 
*/

void VTableError_File(typeError type, const char* message, const char* file_name);

#endif 