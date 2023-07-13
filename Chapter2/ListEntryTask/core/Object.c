#include "Object.h"

#include <stdio.h>

/* don't change ! */
#define OBJ_VFUNC_DISPOSE 0
#define OBJ_VFUNC_HASH 1
#define OBJ_VFUNC_EQUALS 2
#define OBJ_VFUNC_COMPARE 3

void InitObject(Object* obj, objectType type, objectInternalMethod* optTable)
{
	obj->__addr = (__int64)obj;
	obj->type = type;
	obj->internalMethods = ObjectInternalMethods;
	if (optTable != 0) obj->internalMethods = optTable;
}

void printObjectType(Object* obj)
{
	printf("\n%s\n", obj->type());
}

int isObjectsEquals(Object* obj, Object* with) {
	return obj->internalMethods[OBJ_VFUNC_EQUALS](obj, with);
}

__int64 getObjectHash(Object* obj) {
	return obj->internalMethods[OBJ_VFUNC_HASH](obj);
}

void DisposeObject(Object* obj) {
	obj->internalMethods[OBJ_VFUNC_DISPOSE](obj);
}

int compareObjects(Object* obj, Object* with) {
	return obj->internalMethods[OBJ_VFUNC_COMPARE](obj, with);
}

int DestroyObject(Object** objPtr)
{
	if (objPtr == 0) return 0;
	if (*objPtr == 0) return 0;
	if ((*objPtr)->__addr == (__int64)objPtr) {
		debugShowWarning(
			"You should put pointer to pointer (NOT just pointer to!) to Object struct in DisposeObject(...) function!"
		);
	}
	if ((*objPtr)->internalMethods == 0) return 0;
	(*objPtr)->internalMethods[OBJ_VFUNC_DISPOSE](*objPtr);
	free(*objPtr);
	*objPtr = 0;
	return 0;
}