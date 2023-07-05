#include "ParseInt.h"
#include <stdlib.h>

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

int strtoi(IN const char* str, OPT_OUT char** badCharPtr, OUT int* ret)
{
	MAKE_OPTIONAL(char*, badCharPtr);
	if (str == 0 || ret == 0) {
		return STRTOI_ERR_NULL;
	}
	if (str[0] == 0) {
		return (*ret = 0, STRTOI_ERR_NO);
	}
	char sign = 1, base = 10;
	if (str[0] == '-') {
		if(str[1] == '\0')
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);
		sign = -1;
		str = str + 1;
	}

	if (str[0] == '+') {
		str = str + 1;
	}

	if (str[1] == 'x') { //get prefix

		if (str[2] == '\0')
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);

		base = getDigitOrdinal(str[0]);
		if (str[0] == '!') base = 62;
		if(base < 2)
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);

		str = str + 2;
	}

	int length = 0, t = 1, result = 0;
	int t_lim = 2147483647 / base, t_reminder = 2147483647 % base;
	for (; str[length] != '\0'; length++) {};
	for (int i = length - 1; i >= 0; i--) {

		int digit = getDigitOrdinal(str[i], base);
		if(digit < 0 || digit >= base) 
			return (*badCharPtr = str+i, STRTOI_ERR_BAD_CHAR);

		int pre = digit * t;

		if (t >= t_lim) {
			if((i != 0 && digit != 0 && result > t_reminder) 
				|| (pre < t && pre != 0))
				return STRTOI_ERR_OVERFLOW; //last digit?
		}
		else {
			t = t * base;
		}
		int z = pre + result;
		if (z < result) {
			if(z != (INT_MIN) || sign == 1)
				return STRTOI_ERR_OVERFLOW;
		}
		result = z;
	}
	return (*ret = result * sign, STRTOI_ERR_NO);
}

int myitoa(IN char* buf, int bufSize, int value, int base)
{
	if (base < 2 || base > 62) 
		return 0;

	//ascii_base_data = {0: 0-9, 1: A-Z, 2: a-z, 3...5: substract from digit}
	char ascii_base_data[6] = { '0', 'A', 'a', 0, 10, 36}; 
	int iter = 0;
	char* reversedBuf = (char*)calloc(33, sizeof(char));
	if (reversedBuf == 0) return -1;
	char neg_sign = (value < 0);
	value = value * (1 - 2*neg_sign);

	while (value != 0) {
		int digit = value % base;
		int base_offset = ((digit + 16) / 26);
		reversedBuf[iter] =
			ascii_base_data[base_offset] + digit - ascii_base_data[base_offset + 3];
		value /= base;
		iter++;
	}

	if (neg_sign) {
		reversedBuf[iter] = '-'; iter++;
	}

	if (iter > bufSize && buf != 0)
		return 0;

	if (buf == 0) 
		return iter;

	for (int i = 0; i < iter; i++) {
		buf[i] = reversedBuf[iter - 1 - i];
	}
	free(reversedBuf);
	return iter;
}
