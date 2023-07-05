#pragma once
#include "PasswordGenerator.h"
#include "TestUtil.h"

MAKE_TEST(
	PassTest, generatePassword,
	_ARGS char* nFlag, char* m1Flag, char* m2Flag, char* aFlag, char* CFlag,
	_EXCEPT char* contiansSymbols, int length
);

void showGeneratePasswordTest();