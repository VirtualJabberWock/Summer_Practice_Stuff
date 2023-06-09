
#include <stdio.h>
#include <stdlib.h>
#include "StreamUtil.h"

void handleInputFile(FILE* input, FILE* output, char* query, char* replacement);

#define BUFFER_SIZE 61

#include <string.h>

void replaceMatchInFile(int argc, char** argv);

int main(int argc, char** argv) {


	replaceMatchInFile(argc, argv);
	
}

void replaceMatchInFile(int argc, char** argv) {
	FILE* inputFile;
	FILE* outputFile;

	if (argc != 5) {
		printf("Usage: ../FileReplacer.exe <input_file> <output_file> <query> <replaceTo>");
		return 0;
	}
	fopen_s(&inputFile, argv[1], "rb");
	fopen_s(&outputFile, argv[2], "w");
	if (inputFile == 0) {
		printf("Can't find input file! [%s]\nExit...\n", argv[1]);
		return 0;
	}
	if (outputFile == 0) {
		printf("Can't create output file! [%s]\nExit...\n", argv[2]);
		return 0;
	}
	handleInputFile(inputFile, outputFile, argv[3], argv[4]);
	_fcloseall();
	return 0;
}

void handleInputFile(FILE* input, FILE* output, char* match, char* replacement)
{
	char* buffer = calloc(BUFFER_SIZE, sizeof(char));
	if (buffer == 0) return;
	int rSize = strlen(replacement);
	int readedBytes = 1;
	Query* query = NewQuery(match);
	if (query == 0) {
		printf("Memory allocation error!");
		return;
	}
	int queryPos = 0;
	//replaceInStreamFunc = (qSize > BUFFER_SIZE) ? replaceInStreamFast : replaceInStream;
	while (readedBytes > 0) {
		readedBytes = fread(buffer, sizeof(char), BUFFER_SIZE, input);
		replaceInStream(buffer, readedBytes, query, replacement, rSize, output);
		printf("%s\t%d\n", buffer, readedBytes);
	}
	if (query->pos > 0) fwrite(query->match, sizeof(char), query->pos, output);
	free(query);
}

/*

�������.

1. ��������� ������ �� �����,
�������� ��������� ����� ������������������ (�������� �� ����������)
���� �� ������, ��������� �������� � ������ ����.

���� � ������������������ ������������� ������� � ������������� �����������.
��������� ��������� �� ����� ������ ���� ����� ������ ����.

����� ������ � ������������������ (�������� � ����������) �������� �� ���������� ��������� ������.
������ �� ����� ������������ ������� �������� 512 ���� (��� ������ � ������� ������������ ������� fopen ,fread, fwrite, fclose).
������. ���� � ������ "adfkasddkjadfkdaqwed" �������� "ad" �� "12345", �� ������� "12345fkasddkj12345fkdaqwed".

������� ��� ������ 3
1) ����� �������, ��� ������ �������� ������������������ �� ������ 512 ����.
2) � ������ ������ ������� �� ����� ������ ������������ ������ ��� 1024 �����.

������� ��� ������ 4
1) � ������ ������ ������� �� ����� ������ ������������ ������ ��� 512 ����.


*/

