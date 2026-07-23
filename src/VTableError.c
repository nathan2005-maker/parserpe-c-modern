#include "pch.h"
#include "VTableError.h" 

void VTableError_Init(VTableError* error, typeError type, const char* message) {
	if (error == NULL) { exit(EXIT_FAILURE); }

	// Initialize the error structure
	error->type = type;
	error->message = message;

	switch (type) {
	case TYPE_ERROR_FILE_NOT_FOUND:
		fprintf(stderr, "ERROR: FILE NOT FOUND OR READ ERROR\n");
		fprintf(stderr, "  FILE: %s\n", error->system_erro.name_file ? error->system_erro.name_file : "NOT SPECIFIED");
		break;

	case TYPE_ERROR_FILE_INVALID:
		fprintf(stderr, "ERROR: IT IS NOT POSSIBLE TO READ THE FILE\n");
		break;

	case TYPE_ERROR_LIMITE_BUFFER:
		fprintf(stderr, "ERROR: BUFFER LIMIT EXCEEDED\n");
		fprintf(stderr, " HEAD: %s\n", error->limit_buffer_erro.head_buffer ? error->limit_buffer_erro.head_buffer : "NOT SPECIFIED");
		fprintf(stderr, " SIZE FILE: 0x%X bytes\n", error->limit_buffer_erro.limit_buffer);
		fprintf(stderr, " REQUISITIONED OFFSET: 0x%X\n", error->limit_buffer_erro.found_buffer);
		break;

	default:
		fprintf(stderr, "ERROR: UNKNOWN ERROR\n");
		break;
	}
}

void VTableError_File(typeError type, const char* message, const char* file_name) {
	VTableError err = {
		.system_erro = {
			.name_file = file_name,
			.buffer_error = NULL
		}
	};

	VTableError_Init(&err, type, message);
}