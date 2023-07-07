#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "ParseInt.h"
#include "TestUtil.h"

MAKE_TEST(ParseInt, strtoi,
	_ARGS const char* str,
	_EXCEPT int _status, int _number
);

MAKE_TEST(ParseInt, myitoa,
	_ARGS int bufSize, char isBufferNulled, int value, int base,
	_EXCEPT int ret_value, const char* str
);

void showMyIntToStrTests();
void showStrToIntTests();