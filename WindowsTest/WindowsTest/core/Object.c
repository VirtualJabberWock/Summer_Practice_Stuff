#include "Object.h"
#pragma once
#include <stdlib.h>

int isObject(Object* s)
{
	if (s == 0) return 0;
	return (s->header == __OBJECT_HEADER);
}

int checkObjectType(Object* s, ObjectType type)
{
	if (type == 0) return (s == 0);
	if (s == 0) return 0;
	if (s->header != __OBJECT_HEADER) return 0;
	return (s->type == (__int64) type);
}

void standartFree(Object* obj)
{
}

int standartCompare(Object* self, Object* obj, __int64* opt_hashOut)
{
	if (opt_hashOut != NULL) {
		*opt_hashOut = (__int64)self;
	}
	if ((__int64)self == (__int64)obj) {
		return 0;
	}
	if ((__int64)self < (__int64)obj) {
		return -1;
	}
	return 1;
}

int isObjectEquals(Object* obj1, Object* obj2)
{
	if (obj1 == 0 && obj2 == 0) return 1;
	if (obj1 == 0 && obj2 != 0) return 0;
	if (obj1 != 0 && obj2 == 0) return 0;
	if (obj1->type != obj2->type) return 0;
	int a = obj1->compare(obj1, obj2, NULL);
	if (a == 0) return 1;
	return 0;
}

int CompareObjects(Object* base, Object* compareWith)
{
	if (base == 0 && compareWith == 0) return 0;
	if (base == 0 && compareWith != 0) return -1;
	if (base->type != compareWith->type) return -2;
	base->compare(base, compareWith, NULL);
}

__int64 hashObject(Object* obj)
{
	__int64 hash = 0;
	obj->compare(obj, 0, &hash);
	return hash;
}
