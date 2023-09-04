#pragma once 

#include "..\Map.h"


typedef struct tagArrayMap  {

	EXTENDS_MAP;

	Object** values;
	Object** keys;
	int size;
	int capacity;

} ArrayMap;

ArrayMap* NewArrayMap(int capacity);

void FreeArrayMap(ArrayMap* map);