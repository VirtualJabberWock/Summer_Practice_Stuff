#pragma once
#include "Annotations.h"

/*
-m1	        ����������� ����� ������
-m2	        ������������ ����� ������
-n          ����� ������
-c          ���������� �������
-a     	    ������� ��������
-C[aADS]	����� ��������
-help       ������
-p a-10,b-10,...,z-10,...1-40,2-34,... ����������� ��� �������
*/

#pragma pack(1)

typedef struct tagLettersProbabilities {
	char* probabilities;
	int distributedChacne;
	char alphabetData[256];
} LettersProbabilities;

#pragma pack(1)
/*Options for pass generator*/
typedef struct tagGenOptions {

	int passwordsCount;
	int maxPasswordLength;
	int minPasswordLength;
	char isAlphabetCustom;
	const char* customAlphabet;
	int customAlphabetSize;
	char isLatinLowerUsed;
	char isLatinUpperUsed;
	char isDigitsUsed;
	char isSpecialUsed;

	LettersProbabilities* letterProbabilities;

} PassGenOptions;

NEW PassGenOptions* CreateEmptyGenOptions();
int initLetterProbabilities(char* format, OUT PassGenOptions* options);


/*min and max values are include to range*/
int randInt(int min, int max);
NEW char* generatePassword(IN PassGenOptions* options);
int readFlags(int args_c, IN char** argv, OUT PassGenOptions* options);

void showHelpAndExit();
