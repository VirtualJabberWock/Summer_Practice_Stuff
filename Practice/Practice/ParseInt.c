#include "ParseInt.h"

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
		sign = -1;
		str = str + 1;
	}

	if (str[0] == '+') {
		str = str + 1;
	}

	// ascii_data ~ { base_starts[3], digit_offsets[3], base_chunk_limits[3] }
	char ascii_data[9] = { '0', 'A', 'a', 0, 10, 36, 10, 26, 26 };

	if (str[1] == 'x') { //get prefix

		if (str[2] == '\0')
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);

		char offset = (str[0] >> 5) - 1;
		if(offset > 2 || offset < 0)
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);
		base = str[0] - ascii_data[offset] + ascii_data[offset + 3];
		if (base == '!' - '0') base = 62;

		if(base < 2 || base > 62)
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);

		str = str + 2;
	}
	int length = 0, t = 1, result = 0;
	unsigned int int_lim = 2147483648u - (sign == 1);
	int t_lim = int_lim / base;
	
	for (; str[length] != '\0'; length++) {};
	for (int i = length - 1; i >= 0; i--) {
		char offset = (str[i] >> 5) - 1;
		if(offset > 2 || offset < 0)
			return (*badCharPtr = str, STRTOI_ERR_BAD_CHAR);
		int digit = str[i] - ascii_data[offset] + ascii_data[offset+3];
		if (digit == 0) continue;
		if ((digit - ascii_data[offset+3]) >= ascii_data[offset+6] || digit < 0 || digit >= base)
			return (*badCharPtr = str+i, STRTOI_ERR_BAD_CHAR);
		int pre = digit * t;
		if ((t >= t_lim) || t < 0) {
			if(i != 0) return STRTOI_ERR_OVERFLOW; //last digit?
			if(pre < 0 || pre > int_lim) return STRTOI_ERR_OVERFLOW;
		}
		t = t * base;
		unsigned int z = pre + result;
		if (z > int_lim || z < 0) return STRTOI_ERR_OVERFLOW;
		result = z;
	}
	return (*ret = result * sign, STRTOI_ERR_NO);
}

int myitoa(IN char* buf, int bufSize, int value, int base)
{

	if (buf == 0 || bufSize < 0) return 0;
	if (base < 2 || base > 62) return 0;

	//ascii_base_data = {0: 0-9, 1: A-Z, 2: a-z, 3...5: substract from digit}
	char ascii_base_data[6] = { '0', 'A', 'a', 0, 10, 36}; 
	int iter = 0;
	char rBuf[(sizeof(int)*8)+1] = {0}; //binary is a smallest base for int

	while (value != 0) {
		if (bufSize != 0) {
			if (iter >= bufSize) return 0;
			int digit = value % base;
			int base_offset = ((digit + 16) / 26);
			rBuf[iter] =
				ascii_base_data[base_offset] + digit - ascii_base_data[base_offset + 3];
		}
		value /= base;
		iter++;
	}



	if (bufSize == 0) return iter;

	for (int i = 0; i < iter; i++) {
		buf[i] = rBuf[iter - 1 - i];
	}

	return iter;
}
