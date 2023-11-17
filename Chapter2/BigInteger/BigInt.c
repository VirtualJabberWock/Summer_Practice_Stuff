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

BigInt* CloneBigInt(BigInt* from)
{
	BigInt* num = calloc(1, sizeof(BigInt));

	if (num == 0) {
		return 0;
	}

	num->digits = CloneIntArray(from->digits);
	num->sign = from->sign;
	return num;
}

void SetToZero(BigInt* num)
{
	FreeIntArray(num->digits);
	free(num->digits);
	num->digits = CreateIntArray(1);
	num->sign = 0;
}

void FreeBigInt(BigInt* num)
{
	FreeIntArray(num->digits);
	free(num->digits);
	num->digits = 0;
	num->sign = 0;
}

void CopyBigInt(BigInt* dest, BigInt* source)
{
	FreeIntArray(dest->digits);
	free(dest->digits);
	dest->digits = CloneIntArray(source);
	dest->sign = source->sign;
}

BigInt* SliceBigInt(BigInt* num, int from, int to)
{
	BigInt* slice = (BigInt*)calloc(1, sizeof(BigInt));

	if (slice == 0) return panic_mem();
	int size = to - from;
	int offset = from * sizeof(uint32_t);
	if (size >= num->digits->size || size <= 0) {
		free(slice);
		return num;
	}

	slice->digits = CreateIntArray(size);
	slice->digits->size = size;

	memcpy(slice->digits->data, ((char*)num->digits->data) + offset, size * sizeof(int));

	slice->sign = 1;

	return slice;
}

static BigInt* NewEmptyBigInt() {
	BigInt* num = (BigInt*)calloc(1, sizeof(BigInt));
	if (num == 0) {
		return panic_mem();
	}
	num->digits = 0;
	num->sign = 1;
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
	if (num->digits->size == 0) {
		num->sign = 0;
	}
}

/*
if num1 > num2 : return 1
if num2 > num1 : return -1
if num1 == num2 : return 0
*/
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
	if (res->digits != 0) {
		FreeIntArray(res->digits);
		free(res->digits);
	}

	if (bigInt1->sign == 0 || bigInt2->sign == 0) {
		res->digits = CreateIntArray(1);
		res->sign = 0;
		return;
	}

	res->digits = NewFixedIntArray(bigInt1->digits->size + bigInt2->digits->size);
	res->sign = bigInt1->sign * bigInt2->sign;

	if (res->digits->data == 0) {
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

	remove_leading_zeros(res);
}

static void multiply_by_digit(BigInt* num, ULL digit, BigInt* res)
{

	if (res->digits != 0) {
		FreeIntArray(res->digits);
		free(res->digits);
	}

	res->digits = NewFixedIntArray(num->digits->size + 2);
	res->sign = num->sign;

	if (res->digits->data == 0) return;

	uint32_t HL[2] = { 0,0 };
	memcpy(&HL[0], &digit, sizeof(ULL));

	for (int i = 0; i < num->digits->size; i++)
	{
		ULL carry = 0;
		ULL t = ((ULL)HL[0] * (ULL)num->digits->data[i]) + (ULL)res->digits->data[i] + carry;
		carry = t / 0x1'0000'0000L;
		res->digits->data[i] = t % 0x1'0000'0000L;
		t = ((ULL)HL[1] * (ULL)num->digits->data[i]) + (ULL)res->digits->data[i + 1] + carry;
		carry = t / 0x1'0000'0000L;
		res->digits->data[i + 1] = t % 0x1'0000'0000L;
		res->digits->data[i + 2] += carry;
	}

	remove_leading_zeros(res);
}

typedef void* _ptr_value;

static void _swap(_ptr_value* ptrA, _ptr_value* ptrB) {
	_ptr_value temp = *ptrA;
	*ptrA = *ptrB;
	*ptrB = temp;
}

static void _expand_bigint(BigInt* num, uint32_t digit) {
	IntArray* array = num->digits;
	IntArray* clone = CreateIntArray(array->__capacity + 1);
	clone->size = array->size + 1;
	memcpy(clone->data + 1, array->data, array->size * sizeof(int));
	FreeIntArray(num->digits);
	free(num->digits);
	num->digits = clone;
	num->digits->data[0] = digit;
}

// Decimal: [44]44 / 3 (1400) -> 0[24]4 / 3 (1480) -> 00[04] / 3 (1481) -> 1481  
static void division_by_digit(IntArrayRegion* num, ULL digit, BigInt* res)
{
	int w_query = num->size - 2;
	ULL w = (((ULL)num->data[num->size - 1]) << 32) | num->data[w_query];
	ULL q = w / digit;
	ULL r = w % digit;
	*(ULL*)(&res->digits->data[w_query]) = q;
	while (w_query != 0) {
		w_query--;
		w = (r << 32) | (num->data[w_query]);
		q = w / digit; r = w % digit;
		res->digits->data[w_query] = (unsigned int)(q & 0xffff'ffff);
	}
}

static void long_division(BigInt* num, BigInt* dnum, BigInt* res)
{
	if (res == 0) return;
	IntArray* rstack = CreateIntArray(4);
	int ns = num->digits->size;
	ULL frame = (((ULL)num->digits->data[ns-2]) << 32) | num->digits->data[ns-1];
	ULL lead = (ULL)dnum->digits->data[dnum->digits->size - 1];
	ULL pre = frame / lead;
	BigInt* product = NewEmptyBigInt();
	BigInt* rem = NewEmptyBigInt();
	BigInt* frame = SliceBigInt(num, ns - product->digits->size, ns);
	multiply_by_digit(dnum, pre, product);
	SubstractBigInt(frame, product, rem);
	if (rem->sign < 0) {
		char flag = 0;
		BigInt* rem_t = rem;
		BigInt* rem_t2 = NewEmptyBigInt();
		while (rem_t->sign < 0) {
			SumBigInt(rem_t, dnum, rem_t2);
			_swap(&rem_t, &rem_t2);
			pre--; flag = flag ^ 1;
		}
		if (flag) _swap(&rem_t, &rem_t2);
		FreeBigInt(rem_t2);
		free(rem_t2);
	}
	PutToIntArray(rstack, pre);
	_expand_bigint(rem, num->digits->data[ns - product->digits->size]);
	while (1) {

		break;
	}
}

void DivideBigInt(IN BigInt* bigInt1, IN BigInt* bigInt2, OUT BigInt* res)
{
	FreeIntArray(res->digits);
	free(res->digits);

	if (bigInt2->sign == 0 || (bigInt2->digits->size == 1 && bigInt2->digits->data[0] == 0)) {
		panic("Division by zero", -1);
		return;
	}

	if (bigInt1->sign == 0 || (bigInt1->digits->size == 1 && bigInt1->digits->data[0] == 0)) {
		SetToZero(res); return;
	}

	BigInt* dividend = CloneBigInt(bigInt1);
	BigInt* divisor = CloneBigInt(bigInt2);
	dividend->sign = 1;
	divisor->sign = 1;

	int cap = max(bigInt1->digits->__capacity, bigInt2->digits->__capacity) + 1;
	res->digits = CreateIntArray(cap);
	res->digits->size = cap;

	int cx = divisor->digits->size; 
	int dx = dividend->digits->size; 

	IntArrayRegion* ds = GetRegionFromArray(divisor->digits, 0, divisor->digits->size);

	if (dx < cx) {
		SetToZero(res); return;
	}

	if (cx == dx && cx == 1) {
		res->digits->data[0] = bigInt2->digits->data[0] / ds->data[0];
		goto complete;
	}

	if (cx == 1) { 
		division_by_digit(bigInt2->digits->data[0], ds->data[0], res);
	}
	else {
		long_division(dividend, divisor, res);
	}
	
	free(ds);
	FreeBigInt(dividend);
	FreeBigInt(divisor);

complete:
	res->sign = bigInt1->sign * bigInt2->sign;
	remove_leading_zeros(res);
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
	int size = max(bigInt1->digits->__capacity, bigInt2->digits->__capacity) + 1;
	res->digits = NewFixedIntArray(size);

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