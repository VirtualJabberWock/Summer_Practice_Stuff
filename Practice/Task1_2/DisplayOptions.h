#pragma once
#include "getopt.h"
#include <stdio.h>

/*

-i file_name				��� ���������� �����
-o offset					�������� �� ������ �����, � �������� ��������� ���������� (���� �� ������, �� �������� � ������ �����)
-l size						���������� ��������� �� ����� ���� (���� �� ������, �� �������� ��)
-g byte|word|dword|qword	������ ��������� �������� ������: 1 ����, 2 �����, 4 ����� (���� �� ������, �� 1 ����)
-n count					���������� ��������, ��������� � ����� ������ (���� �� ������, �� 16)
-f format_str				�������� ��������� ������, ������������ ������ ������ ������ ������.

*/

typedef enum enumByteDataGroup {
	BYTE = 1,
	WORD = 2,
	DWORD = 4,
	QWORD = 8
} ByteDataGroup;

typedef enum enumOnFlagReadError {
	NO_ERR = 0,
	NULL_DATA = 1,
	ERR_INVALID_NUMBER = 2,
	ERR_NEGATIVE_VALUE = 3,
	ERR_INVALID_FILE = 4,
	ERR_INVALID_GROUP = 5,
	ERR_INVALID_FORMAT = 6,
	ERR_N_FLAG_LIMIT = 7,

} OnFlagReadError;

typedef struct tagDisplayOptions {

	FILE* file;
	int offset; // 0
	int bytesCount; // all
	ByteDataGroup chunkType; //BYTE = 1
	int chunksCount; // 16
	char* format;
	int formatSize;

} DisplayOptions;



DisplayOptions* getOptionsFromFlags(int argc, char** argv);

void disposeDisplayOptions(DisplayOptions** ptrOptions);