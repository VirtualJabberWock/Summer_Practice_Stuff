#pragma once

#include "Util.h"
#include "../Chapter2/Annotations.h"

typedef struct tagBigInt 
{

	IntArray* digits;
	char sign;

} BigInt;

NEW BigInt* NewBigInteger(const char* hexNumber);
NEW BigInt* NewBigIntegerNative(unsigned int value);
NEW BigInt* NewBigZero();
NEW BigInt* CloneBigInt(BigInt* from);

void SetToZero(BigInt* num);
void FreeBigInt(BigInt* num);
void CopyBigInt(BigInt* dest, BigInt* source);

BigInt* SliceBigInt(BigInt* num, int from, int to);

void PrintBigInt(BigInt* num);

void MultiplyBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);
void DivideBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);
void SumBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);
void SubstractBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res);

typedef struct tagBigIntSlice
{

	IntArrayRegion* reg;
	char sign;

} BigIntSlice;

#define BigSliceFromRegion(name, region, sign) BigIntSlice name = {region, sign};