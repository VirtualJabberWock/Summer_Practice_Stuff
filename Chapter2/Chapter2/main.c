#include <stdlib.h>
#include <stdio.h>
#include "BigInteger.h"

int main() {

	unsigned int a = 0x8f'ff'ff'ff;
	unsigned int b = 1;

	printf("%u\n", a - b);

	BigInteger* bigInt1 = NewBigInteger("8fffffff");
	BigInteger* bigInt2 = NewBigInteger("1");
	BigInteger* result = NewBigZero();
	SubstractBigInt(bigInt1, bigInt2, result);
	printBigInt(result);
}