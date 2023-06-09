#pragma once
#include "..\core/Object.h"
#include "..\core\Vector.h"



OBJECT_CLASS_FM(BigInteger,

	NativeVector* iv;
	char sign;
,
	struct tagBigInteger* (*add) (struct tagBigInteger* self, struct tagBigInteger* other);
	struct tagBigInteger* (*substract) (struct tagBigInteger* self, struct tagBigInteger* other);
	struct tagBigInteger* (*init) (struct tagBigInteger* self, struct tagBigInteger* other);

);

BigInteger* NewBigIntegerFromBase16(const char* base16);
void FreeBigInteger(BigInteger* bInt);