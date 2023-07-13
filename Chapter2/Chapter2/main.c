#include <stdlib.h>
#include <stdio.h>
#include "BigInteger.h"

int main() {

	BigInteger* bigInt1 = NewBigInteger("34123431234324123412");
	BigInteger* bigInt2 = NewBigInteger("3123423523454324321");
	BigInteger* result = NewBigZero();
	MultiplyBigInt(bigInt1, bigInt2, result);
	printBigInt(result);
}