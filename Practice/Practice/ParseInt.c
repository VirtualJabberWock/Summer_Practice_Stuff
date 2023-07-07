#include "ParseInt.h"
#include <stdlib.h>



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

	if (str[0] == '-' || str[0] == '+') {
		if(str[1] == '\0')
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);
		sign = 44-str[0];
		str = str + 1;
	}

	if (str[1] == 'x') { //get prefix

		if (str[2] == '\0')
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);

		base = getDigitOrdinal(str[0]);
		if (str[0] == '!') base = 62; //extra symbol
		if(base < 2)
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);

		str = str + 2;
	}

	int result = 0;
	int base_lim = INT_MAX / base; // check before multiplicate somthing by base
	int base_reminder = INT_MAX % base;
	char isExtraEdge = 0;

	for (int i = 0; str[i] != 0; i++) {

		int digit = getDigitOrdinal(str[i], base);
		if(digit < 0 || digit >= base) 
			return (*badCharPtr = str+i, STRTOI_ERR_BAD_CHAR);

		if (result > base_lim || (result == base_lim && digit > base_reminder)) {
			if (sign == -1) { 
				if((digit != (base_reminder + 1)%base) || result > base_lim + 1)
					return STRTOI_ERR_OVERFLOW;
			}
			else {
				return STRTOI_ERR_OVERFLOW;
			}
		}

		result = result * base + digit;
		if (result > 0) isExtraEdge = 1;
	}
	if (isExtraEdge && result == 0) return (*ret = INT_MIN, STRTOI_ERR_NO);
	return (*ret = result * sign, STRTOI_ERR_NO);
}

int myitoa(IN char* buf, int bufSize, int value, int base)
{
	if (base < 2 || base > 62) 
		return 0;

	//ascii_base_data = {0: 0-9, 1: A-Z, 2: a-z, 3...5: substract from digit}
	char ascii_base_data[6] = { '0', 'A', 'a', 0, 10, 36}; 
	int iter = 0;
	char* reversedBuf = (char*)calloc(34, sizeof(char));
	if (reversedBuf == 0) return -1;
	char isNegative = (value < 0);
	value = value * (1 - 2*isNegative);

	while (value != 0) {
		int digit = abs(value % base);
		int base_offset = ((digit + 16) / 26);
		reversedBuf[iter] =
			ascii_base_data[base_offset] + digit - ascii_base_data[base_offset + 3];
		value = abs(value / base);
		iter++;
	}

	if (isNegative) {
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
