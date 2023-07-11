#pragma once

#include <stdio.h>
#include <stdlib.h>


typedef struct tagQuery {

	char* match;
	int size;
	int pos;
	char isTrailing;
	int trailLength;

} Query;

Query* NewQuery(char* match);

void replaceInStream(
	char* block, int blockSize,
	Query* query, 
	char* to, int toSize,
	FILE* outStream
);