#pragma once
#include "Annotations.h"

enum StrToIntError {
	STRTOI_ERR_NULL = -1,
	STRTOI_ERR_NO = 0,
	STRTOI_ERR_BAD_CHAR = 1,
	STRTOI_ERR_OVERFLOW = 2
};

int getDigitOrdinal(char c) {

	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 10;
	}
	if (c >= 'a' && c <= 'z') {
		return c - 'a' + 36;
	}
	return -1;
}

int strtoi(IN const char* str, OPT_OUT char** badCharAddress, OUT int* ret);
int myitoa(IN char* buf, int bufSize, int value, int base);