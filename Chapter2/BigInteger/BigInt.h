#pragma once

#include "Util.h"
#include "../Chapter2/Annotations.h"

typedef struct tagBigInt 
{

	char sign;
	IntArray* digits;

} BigInt;

BigInt* NewBigInteger(const char* hexNumber);
BigInt* NewBigIntegerNative(unsigned int value);
BigInt* NewBigZero();

void PrintBigInt(BigInt* num);

void MultiplyBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);
void DivideBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);
void SumBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);
void SubstractBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);