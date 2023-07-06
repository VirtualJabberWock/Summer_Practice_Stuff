#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "ParseInt.h"
#include "TestUtil.h"
//#include "C:\Users\aquahaze\Downloads\Telegram Desktop\strtoi_itoa.h"

MAKE_TEST(ParseInt, strtoi,
	_ARGS const char* str, 
	_EXCEPT int _status, int _number
) 
{
	printf("\t[%d]: ", __line__);
	char* badChar;
	int number = 0;
	int status =strtoi(str, &badChar, &number);
	if (status == 0 && status == _status) {
		if (number == _number)
			printf("OK! - Number = %d (%s)\n", number, str);
		else
			printf("FAIL! - for %s : %d excepted[%d])\n", str, number, _number);
		return (number == _number);
	}

	if (status != _status)
		printf("FAIL! Status %d != %d(excepted) - ", status, _status);
	else
		printf("OK! - ");

	if (status == STRTOI_ERR_NULL) {
		printf("Error: NULL\n");
	}

	if (status == STRTOI_ERR_BAD_CHAR) {
		printf("Error: Bad char [%c] at %d pos in str: %s\n", *badChar, (int)(badChar - str), str);
	}

	if (status == STRTOI_ERR_OVERFLOW) {
		printf("Error: the number is out of range (-2^31 to 2^31-1): %s\n", str);
	}

	if (status == 0) {
		printf("ok for %s (-_-)\n", str);
	}

	return (status == _status);

}

MAKE_TEST(ParseInt, myitoa,
	_ARGS int bufSize, char isBufferNulled, int value, int base,
	_EXCEPT int EXCEPTED(ret_value), const char* EXCEPTED(str)
) 
{
	char* buf = (isBufferNulled) ? 0 : malloc(sizeof(char) * bufSize);

	printf("\t[%d]: ", __line__);

	int ret_value = myitoa(buf, bufSize, value, base);
	//printf("%s", buf);
	if (ret_value != EXCEPTED(ret_value)) {
		printf("FAIL! - ret value %d != %d(excepted)\n", ret_value, EXCEPTED(ret_value));
		return 0;
	}

	if (bufSize == 0) {
		printf("OK! - [buffer] (%d)\n", value);
		return 1;
	}

	if (ret_value > bufSize && !isBufferNulled) {
		printf("FAIL! - WRONG [ret_value]??? (bad test)\n");
		return 0;
	}

	if (isBufferNulled && ret_value == EXCEPTED(ret_value)) {
		printf("OK! - for [NULL_BUFFER], and (%d) returned %d\n",value, ret_value);
		return 1;
	}

	for (int i = 0; i < ret_value; i++) {
		
		if (buf[i] != EXCEPTED(str)[i]) {
			printf("FAIL! - %s != %s (excepted)\n", buf, EXCEPTED(str));
			return 0;
		}
	}
	if (ret_value == 0) {
		printf("OK! - ret_value = 0, because exceeded bufSize!\n");
		return 1;
	}
	printf_s("OK! - \"");
	fwrite(buf, sizeof(char), ret_value, stdout);
	printf("\" (%d)\n", value);

	return 1;

}

void showMyIntToStrTests() {
	printf("\n=-=-=-= Test myitoa(...):\n\n");
	int a = 1;
	//Basics:
	TEST_(a, ParseInt, myitoa, 9, 0,   1234, 10, /*retval*/ 4, /*str*/ "1234" );
	TEST_(a, ParseInt, myitoa, 3, 0,   1234, 10, /*retval*/ 0, /*str*/ "1234");
	TEST_(a, ParseInt, myitoa, 3, 1,   1234, 10, /*retval*/ 4, /*str*/ "null");
	TEST_(a, ParseInt, myitoa, 5, 0,  -1234, 10, /*retval*/ 5, /*str*/ "-1234");
	//Higher bases:
	TEST_(a, ParseInt, myitoa, 3, 0, 230577, 62, /*retval*/ 3, /*str*/ "xyz");

	if (a == 1) {
		printf("\n\nAll tests PASSED!\n");
	}
	else {
		printf("\n\nOne of tests FAILED!\n");
	}
}

void showStrToIntTests() {

	printf("\n=-=-=-= Test strtoi(...):\n");
	int a = 1, reserved_0 = 0;;
	//Basics:
	printf("\nBasics: \n");
	TEST_(a, ParseInt, strtoi, "-1234", 0, -1234);
	TEST_(a, ParseInt, strtoi, "12-34", 1, 0);
	TEST_(a, ParseInt, strtoi, "1234", 0, 1234);
	TEST_(a, ParseInt, strtoi, "+1234", 0, 1234);
	TEST_(a, ParseInt, strtoi, "Gx1234:", 1, 0);
	TEST_(a, ParseInt, strtoi, "12342319A", 1, 0);
	TEST_(a, ParseInt, strtoi, "1x1234", 1, 0);
	TEST_(a, ParseInt, strtoi, "~x1234", 1, 0);
	TEST_(a, ParseInt, strtoi, "-x1234", 1, 0);
	TEST_(a, ParseInt, strtoi, "-2x", 1, 0);
	TEST_(a, ParseInt, strtoi, "Ax1000000000", 0, 1'000'000'000);
	TEST_(a, ParseInt, strtoi, "Ax000000000000000000000000001", 0, 1);
	printf("\nOverflow tests:\n");
	TEST_(a, ParseInt, strtoi, "Ax123301294", 0, 123301294);
	TEST_(a, ParseInt, strtoi, "Ax2000000000", 0, 2'000'000'000);
	TEST_(a, ParseInt, strtoi, "Ax1000000001", 0, 1'000'000'001);
	TEST_(a, ParseInt, strtoi, "Ax2000000028", 0, 2'000'000'028);
	TEST_(a, ParseInt, strtoi, "Ax2200000000", 2, 2'200'000'000);
	TEST_(a, ParseInt, strtoi, "Ax2147483047", 0, 2147483047);
	TEST_(a, ParseInt, strtoi, "Ax2147483647", 0, 2147483647);
	TEST_(a, ParseInt, strtoi, "Ax2147483648", 2, 2147483648); 
	TEST_(a, ParseInt, strtoi, "Ax2147483649", 2, 2147483649); 
	TEST_(a, ParseInt, strtoi, "Dx282BA4AAA", 0, 2147483647); 
	TEST_(a, ParseInt, strtoi, "Dx282BA4AAB", 2, 2147483648); 
	TEST_(a, ParseInt, strtoi, "Dx25B47736C", 0, 2000000001); 
	TEST_(a, ParseInt, strtoi, "NxDBGL17I", 0, 2000000001); 
	TEST_(a, ParseInt, strtoi, "9147483649", 2, 0); 
	TEST_(a, ParseInt, strtoi, "6000000000", 2, 0); 
	TEST_(a, ParseInt, strtoi, "Ax8999999999", 2, 2147483649);
	TEST_(a, ParseInt, strtoi, "Ax2148483649", 2, 0); 
	TEST_(a, ParseInt, strtoi, "-Ax2147483647", 0, -2147483647);
	TEST_(a, ParseInt, strtoi, "-Ax2147483648", 0, -2147483647-1);
	TEST_(a, ParseInt, strtoi, "-Hx53G7F549", 0, -2147483647-1);
	TEST_(a, ParseInt, strtoi, "-Hx53G7F54A", 2, 0);
	TEST_(a, ParseInt, strtoi, "-Ax2147483649", 2, 2147483649);
	TEST_(a, ParseInt, strtoi, "Bx1264A29", 0, 2188074);
	TEST_(a, ParseInt, strtoi, "Bx12:4A29", 1, 0);
	TEST_(a, ParseInt, strtoi, "Bx12C4A29", 1, 0);
	TEST_(a, ParseInt, strtoi, "Bx12B4A29", 1, 0);
	TEST_(a, ParseInt, strtoi, "", 0, 0);
	printf("\nLimits: \n"); 
	TEST_(a, ParseInt, strtoi, "!xz", 0, 61);
	TEST_(a, ParseInt, strtoi, "!x!", 1, 0);
	TEST_(a, ParseInt, strtoi, "!x10", 0, 62);
	TEST_(a, ParseInt, strtoi, "!xzzzzzzzzzzzz", 2, 0);
	TEST_(a, ParseInt, strtoi, "!xzzzzzzzzzzzzzz", 2, 0);
	/*for (int i = 1; i < 256; i++) {
		char tmp[5] = { 0 };
		sprintf_s(tmp, 5, "!x%c", i);
		TEST_(a, ParseInt, strtoi, tmp, 0, 0);
	}*/
	printf("\nAnnotation:\n\t[%d]: OK! - if not crashed\n", __LINE__);
	a &= (strtoi("2x01010102", 0, &reserved_0) == STRTOI_ERR_BAD_CHAR);

	if (a == 1) {
		printf("\n\nAll tests PASSED!\n");
	}
	else {
		printf("\n\nOne of tests FAILED!\n");
	}
}