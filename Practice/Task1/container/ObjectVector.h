#pragma once

#include "..\core\Object.h"

typedef struct tagObjectVector {
	
	EXTENDS_OBJECT;

	Object** data;
	int size;
	int capacity;

} ObjectVector;

ObjectVector* NewObjectVector(int cap, int size);