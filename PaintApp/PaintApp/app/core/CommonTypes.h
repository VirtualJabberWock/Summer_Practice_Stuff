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

typedef struct tagWrapLong {

	EXTENDS_OBJECT;
	__int64 n;

} Long;

Long* NewLong(__int64 value);

/*
if Object* obj not Integer, returns 0
*/
__int64 GetLong(Object* obj);