
#include <stdio.h>
#include <stdlib.h>
#include "TestArgs.h"
#include "StreamUtil.h"

void handleInputFile(FILE* input, FILE* output, char* query, char* replacement);

#define BUFFER_SIZE 61

#include <string.h>

int main(int argc, char** argv) {

	FILE* inputFile;
	FILE* outputFile;

	if (argc != 5 && !isTestArgsActive()) {
		printf("Usage: ../FileReplacer.exe <input_file> <output_file> <query> <replaceTo>");
		return 0;
	}
	if (isTestArgsActive()) {
		fopen_s(&inputFile, "input.txt", "r");
		fopen_s(&outputFile, "output.txt", "w");
		handleInputFile(inputFile, outputFile,
			"11111111111111111111111111111111111111111111111111111111111111111111111111111111",
			"2222222222222222222222222222222222222222");
		_fcloseall();
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

void handleInputFile(FILE* input, FILE* output, char* query, char* replacement)
{
	char* buffer = calloc(BUFFER_SIZE, sizeof(char));
	if (buffer == 0) return;
	int qSize = strlen(query);
	int rSize = strlen(replacement);
	int readedBytes = 1;
	void (*replaceInStreamFunc)(char*, ...);
	//replaceInStreamFunc = (qSize > BUFFER_SIZE) ? replaceInStreamFast : replaceInStream;
	while (readedBytes > 0) {
		readedBytes = fread(buffer, sizeof(char), BUFFER_SIZE, input);
		replaceInStream(buffer, readedBytes, query, qSize, replacement, rSize, output);
		printf("%s\t%d\n", buffer, readedBytes);
	}
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

