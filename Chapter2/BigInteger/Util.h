#pragma once

int panic(const char* message, int error_code);
int panic_mem();

typedef struct tagIntArray
{

	unsigned int* data;
	int size;
	int __capacity;

} IntArray;

typedef struct tagIntArrayRegion
{

	unsigned int* data;
	int size;
	char reserved[5];

} IntArrayRegion;

IntArray* CreateIntArray(int capacity);
IntArray* NewFixedIntArray(int size);
void PutToIntArray(IntArray* array, unsigned int value);
IntArray* CloneIntArray(IntArray* array);
void CopyIntArray(IntArray* dest, IntArray* source);
void FreeIntArray(IntArray* array);

IntArrayRegion* GetRegionFromArray(IntArray* array, int from, int to);

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
