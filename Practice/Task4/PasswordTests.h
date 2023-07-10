#pragma once
#include "PasswordGenerator.h"
#include "TestUtil.h"
#include "StringUtil.h"

MAKE_TEST(
	PassTest, generatePassword,
	_ARGS char* rawCommandLine,
	_EXCEPT char* contiansSymbols, int length
);

void showGeneratePasswordTest();