#pragma once

#include <stdio.h>
#include <stdlib.h>

void replaceInStream(
	char* block, int blockSize,
	char* query, int querySize, 
	char* to, int toSize,
	FILE* outStream
);

//void replaceInStreamFast(
//	char* block, int blockSize,
//	char* query, int querySize,
//	char* to, int toSize,
//	FILE* outStream
//);