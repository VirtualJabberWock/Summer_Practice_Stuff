#include "PasswordTests.h"
#include <stdio.h>
#include <stdlib.h>
#pragma once

#define TEST_MODULE PassTest

#include <math.h>
#include <string.h>


MAKE_TEST(PassTest, generatePassword,
	_ARGS char* rawCommandLine,
	_EXCEPT char* contiansSymbols, int length
)
{
	int _argc = 0;
	char** test_argv = consoleArgumentsEmulation(rawCommandLine, &_argc);
	PassGenOptions* options = CreateEmptyGenOptions();
	options->passwordsCount = 1;
	char* pass = 0;
	int status = readFlags(_argc, test_argv, options);
	if (status == 1) goto onError;
	pass = generatePassword(options);
	for (int i = 0; i < _argc; i++) {
		free(test_argv[i]);
	}
	free(test_argv);
	if (pass == 0 || contiansSymbols == 0) {
		onError:
		if (contiansSymbols == pass) {
			printf("  \xc8\xcd\xcd  Error: EXCPECTED! \t\t ... \t\t OK!\n");
			return 1;
		}
		else {
			printf("  \xc8\xcd\xcd  Error: NOT EXPECTED \t\t ... \t\t FAIL!\n");
			return 1;
		}
	}
	printf("[%d]: Password: %s", __line__, pass);
	int i = 0;
	int len = strlen(contiansSymbols);
	if (length == 0) printf("        ");
	while (pass[i] != '\0') {
		int s = 0;
		for (int j = 0; j < len; j++) {
			if (pass[i] == contiansSymbols[j]) s = 1;
		}
		if (s == 0) {
			printf("\t\t- invalid chars at password! \t\t FAIL!\n");
			return 0;
		}
		i++;
	}
	if (i != length && length != -1) {
		printf("\t\t- length %d != %d(expected)! \t\t FAIL!\n", i, length);
		return 0;
	}

	printf("\t\t ... \t\t OK!\n");

	free(pass);
	free(options);
	return 1;
}

void showGeneratePasswordTest()
{

	int a = 1;


	TEST_(a, PassTest, generatePassword,
		_ARGS "-n 10 -a 1234 -p 1-50,2-50", _EXCEPT "12", 10
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-n 10 -a 1234 -p 1-30,2-70", _EXCEPT "12", 10
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-n 10 -a abcde", _EXCEPT "abcde", 10
	);

	TEST_(a, PassTest, generatePassword,
		_ARGS "-n 12 -a XSGuard.dll", _EXCEPT "XSGuard.l", 12
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-m1 12 -m2 15 -a XSGuard.dll", _EXCEPT "XSGuard.l", -1
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-m1 22 -m2 20 -a XSGuard.dll", _EXCEPT NULL, -1
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-m1 10 -m2 12 -C a", _EXCEPT "abcdefghijklmnopqrstuwvxyz", -1
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-m1 10 -m2 12 -C D", _EXCEPT "0123456789", -1
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-m3 -n 10 -a -m2", _EXCEPT "-m2", 10
	);

	TEST_(a, PassTest, generatePassword,
		_ARGS "-s -m1 10 -i -m2 14 -C aADS -p A-100", _EXCEPT "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", -1
	);

	TEST_(a, PassTest, generatePassword,
		_ARGS "-m3 -n 10 -a 123456", _EXCEPT "123456", 10
	);

	TEST_(a, PassTest, generatePassword,
		_ARGS "-m3 -n ab -a 123456", _EXCEPT NULL, -1
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-m3 -n 10 -m1 1 -m2 10 -a 123", _EXCEPT NULL, -1
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-m3 -n 10 -a 123 -C a", _EXCEPT NULL, -1
	);

	if (a == 1) {
		printf("\n All Tests PASSED! \n");
	}
	else {
		printf("\n One of tests FAILED! \n");
	}
}
