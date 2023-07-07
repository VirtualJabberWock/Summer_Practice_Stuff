#pragma once
#include "Annotations.h"

enum StrToIntError {
	STRTOI_ERR_NULL = -1,
	STRTOI_ERR_NO = 0,
	STRTOI_ERR_BAD_CHAR = 1,
	STRTOI_ERR_OVERFLOW = 2
};

#define CHECK_ALPHABET(start, end, base_value, c) \
if (c >= start && c <= end) {\
	return c - start + base_value;\
}

#define DEF_ORDINAL -1

static inline int getDigitOrdinal(char c) {

	CHECK_ALPHABET('0', '9', 0, c);
	CHECK_ALPHABET('A', 'Z', 10, c);
	CHECK_ALPHABET('a', 'z', 36, c);
	return DEF_ORDINAL;
}

int strtoi(IN const char* str, OPT_OUT char** badCharAddress, OUT int* ret);
int myitoa(IN char* buf, int bufSize, int value, int base);