#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "ParseInt.h"

int test_ParseInt_strtoi(
	const char* str, 
	int EXCEPTED(status),
	int EXCEPTED(number)
) 
{

	char* badChar;
	int number = 0;
	int status = strtoi(str, &badChar, &number);
	if (status == 0 && status == EXCEPTED(status)) {
		if (number == EXCEPTED(number))
			printf("OK! - Number = %d\n", number);
		else
			printf("FAIL! - for %s : %d excepted[%d])\n", str, number, EXCEPTED(number));
		return (number == EXCEPTED(number));
	}

	if (status != EXCEPTED(status))
		printf("FAIL! Status %d != %d(excepted) - ", status, EXCEPTED(status));
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

	return (status == EXCEPTED(status));

}

int test_ParseInt_myitoa(
	int bufSize,
	int value,
	int base,
	int EXCEPTED(ret_value),
	const char* EXCEPTED(str)
) 
{
	char* buf = malloc(sizeof(char) * bufSize);
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

	if (ret_value >= bufSize) {
		printf("FAIL! - WRONG [ret_value]??? (bad test)");
		return 0;
	}

	for (int i = 0; i < ret_value; i++) {
		
		if (buf[i] != EXCEPTED(str)[i]) {
			printf("FAIL! - %s != %s (excepted)", buf, EXCEPTED(str));
			return 0;
		}
	}
	if (ret_value == 0) {
		printf("OK! - ret_value = 0, because exceeded bufSize!");
		return 1;
	}
	printf_s("OK! - \"");
	fwrite(buf, sizeof(char), ret_value, stdout);
	printf("\" (%d)\n", value);

	return 1;

}

void showMyIntToStrTests() {
	printf("\n\n=-=-=-= Test myitoa(...):\n\n");
	int a = 1;
	//Basics:
	a &= test_ParseInt_myitoa(10, 1234, 10, 4, "1234");
	a &= test_ParseInt_myitoa(10, 5, 2, 3, "101");
	a &= test_ParseInt_myitoa(0, 5, 2, 3, "101");
	a &= test_ParseInt_myitoa(0, 15, 2, 4, "1111");
	a &= test_ParseInt_myitoa(5, 15, 2, 4, "1111");
	a &= test_ParseInt_myitoa(3, 15, 2, 0, "1111");

	if (a == 1) {
		printf("\n\nAll tests PASSED!\n");
	}
	else {
		printf("\n\nOne of tests FAILED!\n");
	}
}

void showStrToIntTests() {
	printf("\n\n=-=-=-= Test strtoi(...):\n\n");
	int a = 1, reserved_0 = 0;;
	//Basics:
	printf("\nBasics: \n");
	a &= test_ParseInt_strtoi("-1234", 0, -1234);
	a &= test_ParseInt_strtoi("12-34", 1, 0);
	a &= test_ParseInt_strtoi("1234", 0, 1234);
	a &= test_ParseInt_strtoi("Gx1234:", 1, 0); //':' next at ASCII '9' should return error, even if base > 10
	a &= test_ParseInt_strtoi("12342319A", 1, 0);
	a &= test_ParseInt_strtoi("1x1234", 1, 0);
	a &= test_ParseInt_strtoi("~x1234", 1, 0);
	a &= test_ParseInt_strtoi("-x1234", 1, 0); 
	a &= test_ParseInt_strtoi("-2x", 1, 0); 
	printf("\nOverflow tests:\n");
	a &= test_ParseInt_strtoi("Ax123301294", 0, 123301294);
	a &= test_ParseInt_strtoi("Ax2000000000", 0, 2'000'000'000);
	a &= test_ParseInt_strtoi("Ax2200000000", 2, 2'200'000'000);
	a &= test_ParseInt_strtoi("Ax2147483647", 0, 2147483647);
	a &= test_ParseInt_strtoi("Ax2147483648", 2, 2147483648); //err
	a &= test_ParseInt_strtoi("Ax2147483649", 2, 2147483649); //err
	a &= test_ParseInt_strtoi("-Ax2147483648", 0, 2147483648);
	a &= test_ParseInt_strtoi("-Ax2147483649", 2, 2147483649);
	a &= test_ParseInt_strtoi("Bx1264A29", 0, 2188074);
	a &= test_ParseInt_strtoi("Bx12:4A29", 1, 0);
	a &= test_ParseInt_strtoi("Bx12C4A29", 1, 0);
	a &= test_ParseInt_strtoi("Bx12B4A29", 1, 0);
	printf("\nLimits: \n");
	a &= test_ParseInt_strtoi("!xz", 0, 61);
	a &= test_ParseInt_strtoi("!x!", 1, 0);
	a &= test_ParseInt_strtoi("!x10", 0, 62);
	a &= test_ParseInt_strtoi("!xzzzzzzzzzzzz", 2, 0);
	a &= test_ParseInt_strtoi("!xzzzzzzzzzzzzzz", 2, 0);
	a &= test_ParseInt_strtoi("!xZzZz", 0, 8578195);
	printf("\nAnnotation:\nOK! - if not crashed\n");
	a &= (strtoi("2x01010102", 0, &reserved_0) == STRTOI_ERR_BAD_CHAR);

	if (a == 1) {
		printf("\n\nAll tests PASSED!\n");
	}
	else {
		printf("\n\nOne of tests FAILED!\n");
	}
}