#include "Object.h"
#pragma once
#include <stdlib.h>

int isObject(Object* s)
{
	return (s->header == __OBJECT_HEADER);
}

int checkObjectType(Object* s, ObjectType type)
{
	if (type == 0) return (s == 0);
	if (s == 0) return 0;
	if (s->header != __OBJECT_HEADER) return 0;
	return (s->type == (__int64) type);
}

void defaultObjectFree(Object* obj)
{
	if (obj->header != __OBJECT_HEADER) return;
	if (obj->_mt != 0) {
		free(obj->_mt);
	}
	obj->_mt = 0;
}
