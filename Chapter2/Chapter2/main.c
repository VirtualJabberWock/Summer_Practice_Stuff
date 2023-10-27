#include <stdlib.h>
#include <stdio.h>
#include "BigInteger.h"

int main() {

	//BigInteger* bigInt1 = NewBigInteger("1000000000");
	//BigInteger* bigInt2 = NewBigInteger("2");
	

	BigInteger* bigInt1 = NewBigInteger("fffffffffffffffffffffffffffff");
	BigInteger* bigInt2 = NewBigInteger("1");

	BigInteger* result = NewBigZero();
	//DivideBigInt(bigInt1, bigInt2, result);
	AddBigInt(bigInt1, bigInt2, result);
	printBigInt(result);
}