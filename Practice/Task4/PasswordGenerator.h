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

typedef struct structGenOptions {

	int passwordsCount;
	int maxPasswordLength;
	int minPasswordLength;
	char isAlphabetCustom;
	const char* custom_alphabet;
	int custom_alphabet_size;
	int alphabet_mask;


} GenOptions;

/*min and max values are include to range*/
int randInt(int min, int max);
NEW char* generatePassword(IN GenOptions* options);
int readFlags(int args_c, IN char** argv, OUT GenOptions* options);

NEW char* CopyString(char* source, OUT int* retSize);