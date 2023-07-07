#include "PasswordTests.h"
#include <stdio.h>
#include <stdlib.h>
#pragma once

#define TEST_MODULE PassTest

#include <math.h>
#include <string.h>


MAKE_TEST(PassTest, generatePassword,
	_ARGS char* nFlag, char* minFlag, char* maxFlag, char* aFlag, char* CFlag,
	_EXCEPT char* contiansSymbols, int length
)
{
	char** test_argv = (char*)calloc(11, sizeof(char*));
	test_argv[0] = CopyString("C:\\path\\generator.exe", 0);
	test_argv[1] = CopyString( (nFlag != 0)   ? "-n"    : "__IGNORED__", 0);
	test_argv[2] = CopyString( (nFlag != 0)   ? nFlag   : "__IGNORED__", 0);
	test_argv[3] = CopyString( (minFlag != 0) ? "-m1"   : "__IGNORED__", 0);
	test_argv[4] = CopyString( (minFlag != 0) ? minFlag : "__IGNORED__", 0);
	test_argv[5] = CopyString( (maxFlag != 0) ? "-m2"   : "__IGNORED__", 0);
	test_argv[6] = CopyString( (maxFlag != 0) ? maxFlag : "__IGNORED__", 0);
	test_argv[7] = CopyString( (aFlag != 0)   ? "-a"    : "__IGNORED__", 0);
	test_argv[8] = CopyString( (aFlag != 0)   ? aFlag   : "__IGNORED__", 0);
	test_argv[9] = CopyString( (CFlag != 0)   ? "-C"    : "__IGNORED__", 0);
	test_argv[10] = CopyString( (CFlag != 0)   ? CFlag   : "__IGNORED__", 0);
	GenOptions options;
	options.passwordsCount = 1;
	readFlags(11, test_argv, &options);
	for (int i = 0; i < 11; i++) {
		free(test_argv[i]);
	}
	free(test_argv);
	char* pass = generatePassword(&options);
	if (pass == 0 || contiansSymbols == 0) {
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
	if (i != length) {
		printf("\t\t- length %d != %d(expected)! \t\t FAIL!\n", i, length);
		return 0;
	}

	printf("\t\t ... \t\t OK!\n");

	free(pass);
	return 1;
}

void showGeneratePasswordTest()
{

	int a = 1;

	TEST_(a, PassTest, generatePassword,
		_ARGS "10", 0, 0, "abcde", 0, _EXCEPT "abcde", 10
	);

	TEST_(a, PassTest, generatePassword,
		_ARGS "12", 0, 0, "XSGuard.dll", 0, _EXCEPT "XSGuard.l", 12
	);
//# generate.exe -m3 -n 10 -a -m2

	TEST_(a, PassTest, generatePassword,
		_ARGS "10", 0, 0, "-m2", 0, _EXCEPT "-m2", 10
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "10", 0, 0, 0, 0,     _EXCEPT 0, 10
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "8", 0, 0, "", 0,     _EXCEPT "0", 0
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-2", 0, 0, "abc", 0, _EXCEPT 0, 0
	);
	
	TEST_(a, PassTest, generatePassword,
		_ARGS "-2", 0, 0, "abc", 0, _EXCEPT 0, 0
	);

	if (a == 1) {
		printf("\n All Tests PASSED! \n");
	}
	else {
		printf("\n One of tests FAILED! \n");
	}
}
