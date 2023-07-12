#pragma once
#include "Object.h"

typedef struct tagWrapInteger {

	EXTENDS_OBJECT;
	int n;

} Integer;

Integer* NewInteger(int value);

/*
if Object* obj not Integer, returns 0
*/
int GetInteger(Object* obj);