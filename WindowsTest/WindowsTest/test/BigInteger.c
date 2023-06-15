#include "BigInteger.h"
#include <stdlib.h>
#include "..\core\DebugUtil.h"
#include "..\core\io\Stream.h"

DEFINE_TYPE(BigInteger)

struct BigInteger_mtable_tag BigInteger_METHODS[] = {
	0,
	0,
	0
};

#include <string.h>


BigInteger* NewBigInteger(const char* base10)
{
	BigInteger* big = calloc(1, sizeof(BigInteger));
	OBJECT_SUPER_FM(BigInteger, big);
	big->free = FreeBigInteger;
	big->iv = NewNativeVector();
	const char* str;
	if (base10[0] == '-') {
		str = base10 + 1; big->sign = -1;
	}
	else {
		str = base10; big->sign = 1;
	}
	NativeVector_push(big->iv, 0);
	//todo
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
