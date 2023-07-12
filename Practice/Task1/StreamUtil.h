#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "container/map\BinarySearchTree.h";

typedef struct tagQuery {

	char* match;
	int size;
	int pos;
	BinaryTree* prefixMap;

} Query;

Query* NewQuery(char* match);

void replaceInStream(
	char* block, int blockSize,
	Query* query, 
	char* to, int toSize,
	FILE* outStream
);

void getPrefixData(Query* query, char* match, int len);