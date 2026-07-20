#ifndef _TOKEN_H
#define _TOKEN_H

#include "pch.h"
#include "include/sv.h"

typedef struct LOCATION{
	const  char* path;
	size_t row;
	size_t column;
} LOCATION;

typedef struct TOKEN {
	LOCATION location;
	String_View sv;
} TOKEN;


#endif /* _TOKEN_H */