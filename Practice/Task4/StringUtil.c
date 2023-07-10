#include "StringUtil.h"
#include <string.h>;
#include <stdio.h>

static void resize(void** ptr, int size, size_t unit) {
	void* temp = realloc(*ptr, size * unit);
	if (temp != 0) *ptr = temp;
	else exit(printf("Can't realloc memory!"));
}

char** splitByChar(char* str, char delimiter, int* retSize)
{

	char** bucket = (char*)calloc(5, sizeof(char*));
	int bucket_s = 0;
	int bucket_cap = 5;
	int i = 0;
	int start = 0, end = 0;
	while (str[i] != '\0') {
		if (str[i] == delimiter) {
			char* tmp = calloc((size_t)end - start + 1, sizeof(char));
			if (tmp == 0) return 0;
			for (int i = 0; i < end - start; i++) {
				tmp[i] = str[i + start];
			}
			if (bucket_s > bucket_cap) {
				bucket_cap *= 2;
				resize(&bucket, bucket_cap, sizeof(char*));
			}
			bucket[bucket_s] = CopyString(tmp, (size_t)end - start);
			bucket_s += 1;
			start = i + 1;
			end = i;
			free(tmp);
		}
		end++;
		i++;
	}
	char* tmp = calloc((size_t)end - start + 1, sizeof(char));
	if (tmp == 0) return 0;
	for (int i = 0; i < end - start; i++) {
		tmp[i] = str[i + start];
	}
	if (bucket_s > bucket_cap) {
		bucket_cap *= 2;
		resize(&bucket, bucket_cap, sizeof(char*));
	}
	bucket[bucket_s] = CopyString(tmp, (size_t)end - start);
	bucket_s += 1;
	*retSize = bucket_s; // [out]
	return bucket;
}

NEW char* CopyString(char* source, OUT int* retSize)
{

	int len = strlen(source);
	char* copy = calloc((size_t)len + 1, sizeof(char));
	if (copy == 0) {
		printf("\n\nFATAL ERROR: Can't alloc Memory\n\n");
		exit(-10);
		return 0;
	}
	copy[len] = '\0';
	for (int i = 0; i < len; i++) {
		copy[i] = source[i];
	}
	if (retSize != 0) *retSize = len;
	return copy;
}

#include <Windows.h>

char** consoleArgumentsEmulation(IN char* raw_command_line, OUT int* outArgCount)
{
	const size_t cSize = strlen(raw_command_line) + 1;
	LPCWSTR wc = calloc(cSize, sizeof(wchar_t));
	size_t* unknown = 0;
	const char* c = 
	mbstowcs(wc, raw_command_line, cSize);
	if (wc == 0) exit(printf("Can't alloc memory!"));
	LPCWSTR* temp = CommandLineToArgvW(wc, outArgCount);
	char** result = calloc(*outArgCount, sizeof(char*));
	for (int i = 0; i < *outArgCount; i++) {
		result[i] = calloc(800, sizeof(char));
;		sprintf_s(result[i], 800, "%ws", temp[i]);
	}
 	free(wc);
	return result;
}
