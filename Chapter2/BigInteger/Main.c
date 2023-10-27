#include <stdio.h>

#include "BigInt.h"

int main(int argc, char** argv)
{

	BigInt* bigInt1 = NewBigInteger("100000001ffffffff");
	BigInt* bigInt2 = NewBigInteger("1");

	BigInt* result = NewBigZero();

	SumBigInt(bigInt1, bigInt2, result);
	PrintBigInt(result);

	return 0;
}