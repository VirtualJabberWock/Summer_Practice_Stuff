#pragma once
#include "Annotations.h"

enum StrToIntError {
	STRTOI_ERR_NULL = -1,
	STRTOI_ERR_NO = 0,
	STRTOI_ERR_BAD_CHAR = 1,
	STRTOI_ERR_OVERFLOW = 2
};



int strtoi(IN const char* str, OPT_OUT char** badCharAddress, OUT int* ret);
int myitoa(IN char* buf, int bufSize, int value, int base);