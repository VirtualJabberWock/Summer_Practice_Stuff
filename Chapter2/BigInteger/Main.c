#include <stdio.h>

#include "BigInt.h"

int main(int argc, char** argv)
{

	BigInt* bigInt1 = NewBigInteger("24d4c87781094e063be7d0efbecf42d1310d980fe7417151cafc60c3f093994db20a732");
	BigInt* bigInt2 = NewBigInteger("81231111");

	BigInt* result = NewBigZero();

	
	DivideBigInt(bigInt1, bigInt2, result);
	PrintBigInt(result);

	return 0;
}