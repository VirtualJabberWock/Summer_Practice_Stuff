#pragma once
#include "Annotations.h"

/*
- m1	    ����������� ����� ������
- m2	    ������������ ����� ������
- n         ����� ������
- c         ���������� �������
- a     	������� ��������
- C[aADS]	����� ��������
*/

#pragma pack(1)

typedef struct structGenOptions {

	int passwordsCount;
	int maxPasswordLength;
	int minPasswordLength;
	char isAlphabetCustom;
	const char* customAlphabet;
	int customAlphabetSize;
	char isLatinLower;
	char isLatinUpper;
	char isDigitsUsed;
	char isSpecialUsed;


} GenOptions;

/*min and max values are include to range*/
int randInt(int min, int max);
NEW char* generatePassword(IN GenOptions* options);
int readFlags(int args_c, IN char** argv, OUT GenOptions* options);

NEW char* CopyString(char* source, OUT int* retSize);