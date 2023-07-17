#include <stdlib.h>
#include <stdio.h>
#include "BigInteger.h"

int main() {

	//BigInteger* bigInt1 = NewBigInteger("1000000000");
	//BigInteger* bigInt2 = NewBigInteger("2");
	

	BigInteger* bigInt1 = NewBigInteger("99999999999999999999999999999");
	BigInteger* bigInt2 = NewBigInteger("1111111");

	BigInteger* result = NewBigZero();
	//DivideBigInt(bigInt1, bigInt2, result);
	SubstractBigInt(bigInt1, bigInt2, result);
	printBigInt(result);
}