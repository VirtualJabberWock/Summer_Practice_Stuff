#include "BigInt.h"

#include <string.h>
#include <stdlib.h>

#define DEBUG 0

BigInt* NewBigInteger(const char* hexNumber)
{
	BigInt* num = calloc(1, sizeof(BigInt));

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
	int digits_count = hexLength / 8 + 1 * ((hexLength + 8) % 8 != 0);

	num->digits = CreateIntArray(digits_count);
	num->digits->size = digits_count;

	if (num->digits == 0) {
		free(num);
		return 0;
	}

	for (int i = 0; i < num->digits->size; i++) {
		char buf[8] = { 0 };
		int chunkSize = hexLength - (i + 1) * 8;
		int shift = max(chunkSize, 0);
		chunkSize = min(0, chunkSize);
		memcpy(buf, hexNumber + shift, 8 + (size_t)chunkSize);
		num->digits->data[i] = strtoll(buf, NULL, 16);
	}

	if (num->digits->size == 0) {
		num->sign = 0;
	}

	if (num->digits->size == 1 && num->digits->data[0] == 0) {
		num->sign = 0;
	}

	return num;
}


BigInt* NewBigIntegerNative(unsigned int value)
{
	BigInt* num = calloc(1, sizeof(BigInt));

	if (num == 0) {
		return 0;
	}
	num->digits = CreateIntArray(4);
	num->digits->data[0] = value;
	num->sign = 1;
	return num;
}

BigInt* NewBigZero() {

	BigInt* num = calloc(1, sizeof(BigInt));

	if (num == 0) {
		return 0;
	}

	num->digits = CreateIntArray(1);
	num->sign = 0;
	return num;
}

void remove_leading_zeros(BigInt* num) {
	for (int i = num->digits->size - 1; i >= 0; i--) {
		if (num->digits->data[i] == 0) {
			num->digits->size--;
		}
		else {
			break;
		}
	}
}

int compare_numbers(IN BigInt* num1, IN BigInt* num2) 
{
	if (num1->digits->size > num2->digits->size) return 1;
	if (num1->digits->size < num2->digits->size) return -1;
	int depth = num1->digits->size - 1;
	while (depth >= 0) {
		if (num1->digits->data[depth] > num2->digits->data[depth]) return 1;
		if (num1->digits->data[depth] < num2->digits->data[depth]) return -1;
		depth--;
	}
	return 0;
}

void add_different_signs(IN BigInt* great, IN BigInt* less, OUT BigInt* res)
{
	char carry = 0;
	for (int i = 0; i < res->digits->size; i++) {
		uint32_t gvalue = (i < great->digits->size) ? great->digits->data[i] : 0;
		uint32_t lvalue = (i < less->digits->size) ? less->digits->data[i] : 0;
		res->digits->data[i] = gvalue - lvalue - carry;
#if DEBUG
		printf("%d, g[i] = %x, l[i] = %x, carry = %d, res[i] = %x\n", i, gvalue, lvalue, carry, res->digits->data[i]);
#endif
		carry = (gvalue < lvalue) || ((gvalue == lvalue) && (carry));
	}
}

void add_same_signs(IN BigInt* great, IN BigInt* less, OUT BigInt* res)
{
	char carry = 0;
	for (int i = 0; i < res->digits->size; i++) {
		uint32_t gvalue = (i < great->digits->size) ? great->digits->data[i] : 0;
		uint32_t lvalue = (i < less->digits->size) ? less->digits->data[i] : 0;
		res->digits->data[i] = lvalue + gvalue + carry;
		carry = (UINT_MAX - lvalue - carry <= gvalue);
	}
}

typedef unsigned long long ULL;

void MultiplyBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res)
{
	if (bigInt1->sign == 0 || bigInt2->sign == 0) {
		if (res->digits != 0) {
			FreeIntArray(res->digits);
			free(res->digits);
		}
		res->digits = CreateIntArray(1);
		res->sign = 0;
		return;
	}

	res->digits->size = bigInt1->digits->size + bigInt2->digits->size;
	res->digits = calloc(res->digits->size, sizeof(unsigned int));
	res->sign = bigInt1->sign * bigInt2->sign;

	if (res->digits == 0) {
		return;
	}

	for (int i = 0; i < bigInt1->digits->size; i++)
	{
		unsigned long long carry = 0;
		int j = 0;
		while (j < bigInt2->digits->size)
		{
			ULL t = ((ULL)bigInt2->digits->data[j] * (ULL)bigInt1->digits->data[i]) + (ULL)res->digits->data[i + j] + carry;
			carry = t / 0x1'0000'0000L;
			res->digits->data[i + j] = t % 0x1'0000'0000L;
			j++;
		}
		res->digits->data[i + j] += carry;
	}

	if (res->digits->size == 1 && res->digits->data[0]) {
		res->sign = 0;
	}

	remove_leading_zeros(res);
}

void DivideBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res)
{

}

void SumBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res)
{
	if (bigInt1 == 0 || bigInt2 == 0 || res == 0) return;

	FreeIntArray(res->digits);
	free(res->digits);
	int cap = max(bigInt1->digits->__capacity, bigInt2->digits->__capacity) + 1;
	res->digits = CreateIntArray(cap);
	res->digits->size = cap;
	
	int cmp = compare_numbers(bigInt1, bigInt2);
	BigInt* greatRef = (cmp >= 0) ? bigInt1 : bigInt2;
	BigInt* lessRef = (cmp >= 0) ? bigInt2 : bigInt1;

	if (bigInt1->sign == bigInt2->sign) {
		res->sign = bigInt1->sign;
		add_same_signs(greatRef, lessRef, res);
	}
	else {
		if (cmp == 0) {
			res->sign = 0; goto complete;
		}
		res->sign = greatRef->sign;
		add_different_signs(greatRef, lessRef, res);
	}

	complete:
	remove_leading_zeros(res);
}

void SubstractBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res)
{
	if (bigInt1 == 0 || bigInt2 == 0 || res == 0) return;

	FreeIntArray(res->digits);
	free(res->digits);
	int cap = max(bigInt1->digits->__capacity, bigInt2->digits->__capacity) + 1;
	res->digits = CreateIntArray(cap);
	res->digits->size = cap;

	int cmp = compare_numbers(bigInt1, bigInt2);
	BigInt* greatRef = (cmp >= 0) ? bigInt1 : bigInt2;
	BigInt* lessRef = (cmp >= 0) ? bigInt2 : bigInt1;

	if (bigInt1->sign == bigInt2->sign) {
		res->sign = bigInt1->sign;
		add_same_signs(greatRef, lessRef, res);
	}
	else {
		if (cmp == 0) {
			res->sign = 0; goto complete;
		}
		res->sign = greatRef->sign;
		add_different_signs(greatRef, lessRef, res);
	}

complete:
	remove_leading_zeros(res);
}

#include <stdio.h>

void PrintBigInt(BigInt* bigInt)
{
	if (bigInt->digits->size == 0) {
		printf("0");
		return;
	}
	if (bigInt->sign < 0) {
		printf("-");
	}
	long long t = (long long)bigInt->digits->data[bigInt->digits->size - 1];
	if (t > 0 || bigInt->digits->size == 1) {
		printf("%llx", t);
	}
	for (int i = bigInt->digits->size - 2; i >= 0; i--) {
		printf("%08llx", (long long)bigInt->digits->data[i]);
	}
}