#ifndef PCH_H
#define PCH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>


#include "VTableError.h"

static inline bool ispe32(IMAGE_DOS_HEADER buffer, char* argv[]) {
	//"PE\0\0" é IMAGE_NT_SIGNATURE (0x00004550)
	if (buffer.e_magic != IMAGE_DOS_SIGNATURE) {
		VTableError_File(TYPE_ERROR_FILE_INVALID,
			"FILE IS NOT A VALID DOS EXECUTABLE",
			argv[1]);
		return EXIT_FAILURE;
	}
	puts("FILE IS A VALID DOS EXECUTABLE");
	return EXIT_SUCCESS;
}


#endif
