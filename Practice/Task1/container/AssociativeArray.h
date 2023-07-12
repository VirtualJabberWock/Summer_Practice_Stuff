#pragma once 
#include "..\core\Object.h"

typedef struct tagArrayMapVTABLE {

	void (*emplace)(Object* map, Object* key, Object* value);
	Object* (*get)(Object* map, Object* key);
	Object* (*getByStr)(Object* map, const char* key);

} ArrayMapVTABLE;

typedef struct tagArrayMap  {

	ArrayMapVTABLE* _;
	Object** values;
	Object** keys;
	int size;
	int capacity;

} ArrayMap;

ArrayMap* NewArrayMap(int capacity);
