#include <stdlib.h>
#include <stdio.h>
#include "BigInteger.h"

int main() {

	//BigInteger* bigInt1 = NewBigInteger("1000000000");
	//BigInteger* bigInt2 = NewBigInteger("2");
	

	BigInteger* bigInt1 = NewBigInteger("b60b9752c5f9876369d048d159e258c");
	BigInteger* bigInt2 = NewBigInteger("2222333322223333");

	BigInteger* result = NewBigZero();
	//DivideBigInt(bigInt1, bigInt2, result);
	SubstractBigInt(bigInt1, bigInt2, result);
	printBigInt(result);
}