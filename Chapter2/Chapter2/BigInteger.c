#include "BigInteger.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

BigInt* NewBigInteger(const char* hexNumber)
{
	BigInteger* num = calloc(1, sizeof(BigInteger));

	if (num == 0) {
		return 0;
	}

	num->sign = 1;

	if (hexNumber[0] == '-' || hexNumber[0] == '+') {
		num->sign = 44 - hexNumber[0]; // '-' - 45, '+' - 43 => -1 or 1
		hexNumber += 1;
	}

	if (hexNumber[0] == '0' && hexNumber[1] == 'x') {
		hexNumber += 2;
	}

	int hexLength = strlen(hexNumber);
	num->digitsCount = hexLength/8 + 1 * ((hexLength + 8) % 8 != 0);

	num->capacity = num->digitsCount;

	num->digits = (unsigned int*) calloc(num->digitsCount, sizeof(int));

	if (num->digits == 0) {
		free(num);
		return 0;
	}

	for (int i = 0; i < num->digitsCount; i++) {
		char buf[8] = { 0 };
		int chunkSize = hexLength - (i + 1) * 8;
		int shift = max(chunkSize, 0);
		chunkSize = min(0, chunkSize);
		memcpy(buf, hexNumber + shift, 8+(size_t)chunkSize);
		num->digits[i] = strtoll(buf, NULL, 16);
	}

	if (num->digitsCount == 0) {
		num->sign = 0;
	}

	if (num->digitsCount == 1 && num->digits[0] == 0) {
		num->sign = 0;
	}

	return num;
}

BigInt* NewBigIntegerNative(unsigned int value)
{
	BigInteger* num = calloc(1, sizeof(BigInteger));

	if (num == 0) {
		return 0;
	}
	num->capacity = 1;
	num->digits = calloc(num->capacity, sizeof(unsigned int));
	num->digits[0] = value;
	num->digitsCount = 1;
	num->sign = 1;
	return num;
}

BigInt* NewBigZero()
{
	BigInteger* num = calloc(1, sizeof(BigInteger));

	if (num == 0) {
		return 0;
	}

	num->digits = 0;
	num->digitsCount = 0;
	num->capacity = 0;
	num->sign = 0;
	return num;
}


void printBigInt(BigInt* bigInt)
{
	if (bigInt->digitsCount == 0) {
		printf("0");
		return;
	}
	if (bigInt->sign < 0) {
		printf("-");
	}
	long long t = (long long)bigInt->digits[bigInt->digitsCount - 1];
	if (t > 0) {
		printf("%llx", t);
	}
	for (int i = bigInt->digitsCount - 2; i >= 0; i--) {
		printf("%08llx", (long long) bigInt->digits[i]);
	}
}

void CopyBigInt(IN BigInt* source, OUT BigInt* destination)
{
	destination->sign = source->sign;
	destination->digitsCount = source->digitsCount;
	destination->capacity = source->capacity;
	if (destination->digits != 0) {
		free(destination->digits);
	}
	destination->digits = calloc(source->capacity, sizeof(unsigned int));
	if (destination->digits == 0) return;
	for (int i = 0; i < source->digitsCount; i++) {
		destination->digits[i] = source->digits[i];
	};
}

void CopyBigIntChunk(IN BigInt* source, OUT BigInt* destination, int size)
{
	destination->sign = source->sign;
	destination->digitsCount = size;
	destination->capacity = source->capacity;
	if (destination->digits != 0) {
		free(destination->digits);
	}
	destination->digits = calloc(source->capacity, sizeof(unsigned int));
	if (destination->digits == 0) return;
	for (int i = 0; i < size; i++) {
		destination->digits[i] = source->digits[i];
	};
}

void MoveBigInt(IN BigInt* source, OUT BigInt* destination, int fixed_size)
{
	destination->sign = source->sign;
	destination->digitsCount = fixed_size;
	destination->capacity = source->capacity;
	for (int i = 0; i < fixed_size; i++) {
		destination->digits[i] = source->digits[i];
	};
}

static int validateOperation(BigInt* a, BigInt* b, BigInt* result) {

	if (result == 0 || a == 0 || b == 0) {
		return 0;
	}

	if (result->digits != 0) {
		printf("\n\n[Warning] [BigInt]: result value should be Big Zero!\n\n");
		return 0;
	}

	return 1;
}

static void setToBigZero(BigInt* num) {
	num->sign = 0;
	num->digitsCount = 0;
	if (num->digits != 0) {
		free(num->digits);
	}
	num->digits = 0;
	num->capacity = 0;
}

void removeLeadingZeros(BigInt* bigInt) {
	int t = bigInt->digitsCount;
	for (int i = bigInt->digitsCount - 1; i >= 0; i--) {
		if (bigInt->digits[i] == 0) {
			t--;
		}
		else {
			break;
		}
	}
	bigInt->digitsCount = t;
}

static int AbsoluteCmpForEqualDigits(unsigned int* high, unsigned int* low, int size)
{
		int depth = size - 1;
		while (depth >= 0) {
			if (high[depth] > low[depth]) return 1;
			if (high[depth] < low[depth]) return -1;
			depth--;
		}
		return 0;
}

static int BitIntAbsoluteCmp(IN BigInt* high, IN BigInt* low)
{
	if (high->digitsCount == low->digitsCount) {
		return AbsoluteCmpForEqualDigits(high->digits, low->digits, high->digitsCount);
	}
	if (high->digitsCount > low->digitsCount) {
		return 1;
	}
	else {
		return -1;
	}
}

static void addPositive(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res) {

	BigInt* a, * b; //references

	if (bigInt1->digitsCount > bigInt2->digitsCount) {
		a = bigInt1;
		b = bigInt2;
	}
	else {
		b = bigInt1;
		a = bigInt2;
	}

	// Then: a.digitsCount > b.digitsCount;

	res->digits = calloc((size_t)a->digitsCount + 1, sizeof(int));
	res->capacity = a->digitsCount + 1;
	if (res->digits == 0) {
		return;
	}

	unsigned int* resNextRef = res->digits + 1; //IDE moment

	for (int i = 0; i < a->digitsCount; i++) {

		if (i >= b->digitsCount) {
			if (res->digits[i] && (a->digits[i] == 0xfffffffe)) {
				resNextRef[i] = 1;
			}
			res->digits[i] = a->digits[i] + res->digits[i];
			continue;
		}

		if (a->digits[i] > UINT_MAX - b->digits[i] - res->digits[i]) {
			resNextRef[i] = 1;
		}

		res->digits[i] = a->digits[i] + b->digits[i] + res->digits[i];
	}

	res->digitsCount = a->digitsCount + (res->digits[a->digitsCount] > 0);
}

static void highSubLower(unsigned int* high, int highSize, unsigned int* low, int lowSize, BigInt* res) {

	unsigned int carry = 0;

	for (int i = 0; i < highSize; i++) {
		unsigned int b_ = (i >= lowSize) ? 0 : low[i];
		res->digits[i] = high[i] - b_ - carry;
		carry = 0;
		if (high[i] < b_ + carry) {
			carry = 1;
		}
	}
}

static void addDiffrentSigns(IN BigInt* positive, IN BigInt* negative, OUT BigInt* res) {


	int maxDigitsCount = max(positive->digitsCount, negative->digitsCount);

	res->sign = BitIntAbsoluteCmp(positive, negative);
	if (res->sign == 0) {
		res->sign = 1;
		return; //We get zero;
	}

	res->digits = calloc((size_t)maxDigitsCount + 1, sizeof(int));
	res->capacity = maxDigitsCount + 1;
	if (res->digits == 0) {
		return;
	}

	unsigned int* resNextRef = res->digits + 1; //IDE moment

	BigInt *a, *b; //references

	if (res->sign > 0) {
		a = positive;
		b = negative;
	}
	else {
		a = negative;
		b = positive;
	}

	unsigned int carry = 0;

	for (int i = 0; i < maxDigitsCount; i++) {
		unsigned int b_ = (i >= b->digitsCount) ? 0 : b->digits[i];
		res->digits[i] = a->digits[i] - b_ - carry;
		carry = 0;
		if (a->digits[i] < b_ + carry) {
			carry = 1;
		}
	}

	res->digitsCount = maxDigitsCount + 1;
	removeLeadingZeros(res);
}

typedef unsigned long long ULL;

void MultiplyBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res)
{
	if (bigInt1->sign == 0 || bigInt2->sign == 0) {
		setToBigZero(res);
		return;
	}

	res->digitsCount = bigInt1->digitsCount + bigInt2->digitsCount;
	res->digits = calloc(res->digitsCount, sizeof(unsigned int));
	res->sign = bigInt1->sign * bigInt2->sign;

	if (res->digits == 0) {
		return;
	}

	for (int i = 0; i < bigInt1->digitsCount; i++)
	{
		unsigned long long carry = 0;
		int j = 0;
		while (j < bigInt2->digitsCount)
		{
			ULL t = ((ULL)bigInt2->digits[j] * (ULL)bigInt1->digits[i]) + (ULL)res->digits[i + j] + carry;
			carry = t / 0x1'0000'0000L;
			res->digits[i + j] = t % 0x1'0000'0000L;
			j++;
		}
		res->digits[i + j] += carry;
	}

	if (res->digitsCount == 1 && res->digits[0]) {
		res->sign = 0;
	}
}

void DivideBigInt(IN BigInt* a, IN BigInt* b, OUT BigInt* res)
{
	if (a->sign == 0) {
		setToBigZero(res);
		return;
	}

	if (b->sign == 0) {
		printf("\nError: Division by zero!\n");
		return;
	}

	res->digitsCount = a->digitsCount - b->digitsCount;
	res->digits = calloc(res->digitsCount, sizeof(unsigned int));
	res->sign = a->sign * b->sign;

	int resPos = res->digitsCount - 1;
	int chunk = b->digitsCount;
	int shift = a->digitsCount - 1;
	int mode = 0;
	BigInt* aClone = NewBigZero();
	CopyBigInt(a, aClone);
	BigInt* buffer = NewBigIntegerNative(1);
	BigInt* swapBuffer = NewBigIntegerNative(1);
	while(resPos >= 0 && shift >= 0)
	{
		mode = AbsoluteCmpForEqualDigits(aClone->digits+shift, b->digits, chunk);
		if (mode = 0) {
			res->digits[resPos] += 1;
			resPos--;
			//setToBigZero(carry);
			shift -= chunk;
			continue;
		}
		else if (mode > 0) {
			int count = 1;
			CopyBigInt(aClone, buffer, chunk);
			CopyBigInt(aClone, swapBuffer);
			while (swapBuffer->sign > 0) {
				highSubLower(buffer->digits, chunk, b->digits, chunk, swapBuffer);
				if (swapBuffer->sign <= 0) break;
				MoveBigInt(swapBuffer, buffer, a->digitsCount);
				count++;
			}
			
		}
		else {
			  
		}

	}

	if (buffer->digits != 0) free(buffer->digits);
	if (swapBuffer->digits != 0) free(swapBuffer->digits);
	free(buffer);
	free(swapBuffer);
	
}

void AddBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res)
{
	
	if (!validateOperation(bigInt1, bigInt2, res)) {
		return;
	}

	if (bigInt1->sign == 0) {
		CopyBigInt(bigInt2, res);
		return;
	}
	
	if (bigInt2->sign == 0) {
		CopyBigInt(bigInt1, res);
		return;
	}

	if (bigInt1->sign == bigInt2->sign) {
		addPositive(bigInt1, bigInt2, res);
		res->sign = bigInt1->sign;
		return;
	}

	if (bigInt1->sign > bigInt2->sign) {
		addDiffrentSigns(bigInt1, bigInt2, res);
	}
	else {
		addDiffrentSigns(bigInt2, bigInt1, res);
	}

	if (res->digitsCount == 1 && res->digits[0]) {
		res->sign = 0;
	}
}

void SubstractBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res)
{
	if (!validateOperation(bigInt1, bigInt2, res)) {
		return;
	}

	if (bigInt1->sign == 0) {
		CopyBigInt(bigInt2, res);
		res->sign = -1 * res->sign;
		return;
	}

	if (bigInt2->sign == 0) {
		CopyBigInt(bigInt1, res);
		return;
	}

	if (bigInt1->sign == bigInt2->sign) { // a + (-b)
		bigInt2->sign = -1 * bigInt2->sign;
		addDiffrentSigns(bigInt1, bigInt2, res);
		bigInt2->sign = -1 * bigInt2->sign;
		return;
	}

	res->sign = bigInt1->sign;
	addPositive(bigInt1, bigInt2, res); // a -(-b) OR -a - b => (a+b) * sign(a)

	if (res->digitsCount == 1 && res->digits[0]) {
		res->sign = 0;
	}

}

