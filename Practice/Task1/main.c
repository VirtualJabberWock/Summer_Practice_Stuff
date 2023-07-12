
#include <stdio.h>
#include <stdlib.h>
#include "TestArgs.h"
#include "StreamUtil.h"

void handleInputFile(FILE* input, FILE* output, char* query, char* replacement);

#define BUFFER_SIZE 61

#include <string.h>
#include "core/Object.h"
#include "core/String.h"
#include "core/CommonTypes.h"
#include "container\map\BinarySearchTree.h"
#include "container\map\HashMap.h"

void testFunc(Integer* obj) {
	int n = GetInteger(obj);
	printf("%d\n", n);
}

char* generateRandomString(int len) {
	char* a = calloc(len + 1, sizeof(char));
	for (int i = 0; i < len; i++) {
		a[i] = rand() % 256;
	}
	a[len] = 0;
	return a;
}

void testHashMap() {

	IMap* hashMap = NewHashMap();

	mapEmplace(hashMap, NewString("username"), NewString("John"));
	mapEmplace(hashMap, NewString("password"), NewString("qwerty12345"));
	mapEmplace(hashMap, NewString("age"), NewString("27"));

	String* passkey = NewString("password");

	String* password = mapGet(hashMap, passkey);

	DestroyObject(&passkey);

	printf("Password: %s", password->data);

	DestroyObject(&hashMap);
}

#include <time.h>

int main(int argc, char** argv) {

	IMap* map = NewHashMap();
	
	

	//printf("%lld", ticks);

	String* key;
	String* value;

	mapEmplace(map, NewString("user"), NewString("abc"));

	for (int i = 0; i < 1'000'000; i++) {
		key = NewString(generateRandomString(10));
		value = NewString(generateRandomString(10));
		mapEmplace(map, key, value);
	}

	printf("Search:\n");

	__int64 ticks = time(0);

	for (int i = 0; i < 1'000'000; i++) {
		value = mapGet(map, key);
	}

	value = mapGet(map, NewString("user"));

	printf("%s\n", value->data);

	ticks = time(0) - ticks;

	printf("%lld", ticks);

	DestroyObject(&map);

	//replaceMatchInFile(argc, argv);

}

void replaceMatchInFile(int argc, char** argv) {
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
	ClearTree(query->prefixMap);
	free(query->prefixMap);
	free(query);
}

/*

Задания.

1. Прочитать данные из файла,
заменить вхождения одной последовательности (исходной на заменяющую)
байт на другую, результат записать в другой файл.

Файл и последовательности произвольного размера и произвольного содержимого.
Запрещено считывать из файла каждый байт более одного раза.

Имена файлов и последовательности (исходную и заменяющую) получать из аргументов командной строки.
Чтение из файла осуществлять блоками размером 512 байт (для работы с файлами использовать функции fopen ,fread, fwrite, fclose).
Пример. Если в тексте "adfkasddkjadfkdaqwed" заменить "ad" на "12345", то получим "12345fkasddkj12345fkdaqwed".

Условия для оценки 3
1) Можно считать, что размер исходной последовательности не больше 512 байт.
2) В памяти нельзя хранить из файла данных одновременно больше чем 1024 байта.

Условия для оценки 4
1) В памяти нельзя хранить из файла данных одновременно больше чем 512 байт.


*/

