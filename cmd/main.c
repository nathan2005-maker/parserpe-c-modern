#include "pch.h"

int main(int argc, char* argv[]) {
	FILE* fh = NULL;

	if (argc != 2) {
		VTableError_File(TYPE_ERROR_FILE_NOT_FOUND,
			"INSUFFICENT ARGUMENTS",
			"NOT FILE THE PAST AN ARGUMENT");
		return EXIT_FAILURE;
	}

	fh = fopen(argv[1], "rb");
	if (fh == NULL) {
		VTableError_File(TYPE_ERROR_FILE_NOT_FOUND,
			"NOT POSSIBLE TO OPEN THE FILE",
			argv[1]);
		return EXIT_FAILURE;
	}

	IMAGE_DOS_HEADER buffer;

	if (fread(&buffer, sizeof(IMAGE_DOS_HEADER), 1, fh) != 1) {
		VTableError_File(TYPE_ERROR_FILE_INVALID,
			"NOT POSSIBLE TO READ 32 BYTES FROM THE FILE",
			argv[1]);
		fclose(fh);
		return EXIT_FAILURE;
	}

	if (!ispe32(buffer, argv)) {
		fclose(fh);
		return EXIT_FAILURE;
	}

	fclose(fh);
	return EXIT_SUCCESS;
}