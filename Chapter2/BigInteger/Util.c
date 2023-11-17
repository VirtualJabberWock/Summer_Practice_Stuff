#include "Util.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

int panic(const char* message, int error_code)
{
	printf("\nError: %s!\n", message);
	exit(error_code);
	return 0;
}

int panic_mem()
{
	printf("\nMemory allocation error!\n");
	exit(-1);
	return 0;
}

IntArray* CreateIntArray(int capacity)
{
	IntArray* array = (IntArray*)calloc(1, sizeof(IntArray));
	if (array == 0) {
		return panic_mem();
	}
	array->size = 0;
	array->__capacity = max(capacity, 4);
	array->data = (unsigned int*)calloc(array->__capacity, sizeof(unsigned int));
	if (array->data == 0) {
		return panic_mem();
	}
	return array;
}

IntArray* NewFixedIntArray(int size)
{
	IntArray* array = (IntArray*)calloc(1, sizeof(IntArray));
	if (array == 0) {
		return panic_mem();
	}
	array->size = size;
	array->__capacity = size;
	array->data = (unsigned int*)calloc(array->__capacity, sizeof(unsigned int));
	if (array->data == 0) {
		return panic_mem();
	}
	return array;
}

void FreeIntArray(IntArray* array)
{
	if (array->data != 0) {
		free(array->data);
	}
	array->data = 0;
	array->size = 0;
}

IntArrayRegion* GetRegionFromArray(IntArray* array, int from, int to)
{
	IntArrayRegion* region = (IntArrayRegion*) calloc(1, sizeof(IntArrayRegion));

	if (region == 0) return panic_mem();

	if (from >= array->size) {
		return region;
	}
	region->data = array->data + from;
	region->size = max(0, to - from);

	memcpy(&(region->reserved[0]), (char*)array->__capacity, 4);

	return region;
}

void PutToIntArray(IntArray* array, unsigned int value)
{
	if (array->size >= array->__capacity) {
		array->__capacity = array->__capacity * 2;
		void* ptr = realloc(array->data, sizeof(unsigned int) * array->__capacity);
		if (ptr == 0) {
			panic_mem();
			return;
		}
		array->data = (unsigned int*) ptr;
	}
	array->data[array->size] = value;
	array->size++;
}

IntArray* CloneIntArray(IntArray* array)
{
	IntArray* clone = CreateIntArray(array->__capacity);
	clone->size = array->size;
	memcpy(clone->data, array->data, array->size * sizeof(int));
	return clone;
}

void CopyIntArray(IntArray* dest, IntArray* source)
{
	if (dest->__capacity < source->__capacity) return;
	if (dest->size < source->size) return;
	for (int i = 0; i < source->size; i++) {
		dest->data[i] = source->data[i];
	}
}
