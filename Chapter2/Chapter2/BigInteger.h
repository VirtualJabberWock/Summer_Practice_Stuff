#pragma once

#include "Annotations.h"

typedef struct tagBigInteger {

	unsigned int* digits;
	int digitsCount;
	int capacity;
	char sign;

} BigInteger, BigInt;

BigInt* NewBigInteger(const char* hexNumber);
BigInt* NewBigZero();

void printBigInt(BigInt* bigInt);

void CopyBigInt(IN BigInt* source, OUT BigInt* destination);

void MultiplyBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);
void AddBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);
void SubstractBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);