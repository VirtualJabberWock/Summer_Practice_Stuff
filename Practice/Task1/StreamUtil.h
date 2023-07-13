#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "container\BinarySearchTree.h";

typedef struct tagQueryPrefixData {

	int count;
	int* positions;
	int* lengths;

} QueryPrefixData;

typedef struct tagQuery {

	char* match;
	int size;
	int pos;
	QueryPrefixData* pdata;

} Query;

Query* NewQuery(char* match);

void replaceInStream(
	char* block, int blockSize,
	Query* query, 
	char* to, int toSize,
	FILE* outStream
);

QueryPrefixData* getPrefixData(char* match, int len);

int getPrefixForPos(Query* q,int pos);