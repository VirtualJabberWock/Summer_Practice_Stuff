#include "BigInteger.h"
#include <stdlib.h>
#include "..\core\DebugUtil.h"
#include "..\core\CommonTypes.h"

DEFINE_TYPE(BigInteger)

struct BigInteger_mtable_tag BigInteger_METHODS[] = {
	0,
	0,
	0
};

#include <string.h>

static const int int32_hex_symbols_c = sizeof(int) * 2;
#define HALF_BYTE_SHIFT 4 // 8 bits for full byte

BigInteger* NewBigIntegerFromBase16(const char* base16)
{
	BigInteger* big = calloc(1, sizeof(BigInteger));
	OBJECT_SUPER_FM(BigInteger, big);
	big->free = FreeBigInteger;
	const char* str;
	if (base16[0] == '-') {
		str = base16 + 1; big->sign = -1;
	}
	else {
		str = base16; big->sign = 1;
	}
	if (str[0] != '\0') {
		if (str[0] == '0' && str[1] == 'x') str = str + 2; // for '0x' prefix
	}
	int i = 0;
	big->iv = NewNativeVector(3);
	int chunk = 0; int pos = 32;
	while (str[i] != '\0') {
		byte_t digit = str[i] - '0';
		if (digit < 10) __pass
		else if (digit >= 17 && digit <= 22) digit -= 7; //abcdef
		else if (digit >= 49 && digit <= 54) digit -= 39; //ABCDEF
		else throw NUMBER_FORMAT_EXCEPTION;
		chunk = digit << pos;
		pos -= 4;
		if (i % int32_hex_symbols_c == int32_hex_symbols_c - 1) {
			NativeVector_push(big->iv, chunk);
			pos = 32;
			chunk = 0;
		}
		i += 1;
	}
	if(chunk != 0) NativeVector_push(big->iv, chunk);
	return big;
}

void FreeBigInteger(BigInteger* bInt)
{
	if (bInt == 0) return;
	if (bInt->iv != 0) {
		if (bInt->iv->data != 0) {
			free(bInt->iv->data);
			bInt->iv->data = 0;
		}
		free(bInt->iv);
		bInt->iv = 0;
	}
}
