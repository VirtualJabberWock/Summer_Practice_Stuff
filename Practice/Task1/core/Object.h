#pragma once

#include <stdlib.h>
#include "DebugUtil.h"

#ifdef ENVIRONMENT64
typedef __int64 pointer_value;
typedef __int64 qword;
#else
typedef __int32 pointer_value;
typedef __int32 qword;
#endif

#define DEFINE_OBJ_TYPE(type) \
__declspec(dllexport) const char* type##_TYPE(){return #type;}; \
static objectInternalMethod Object_##type##_InternalMethods[4] = { \
	ObjectDefaultDispose, ObjectDefaultHash, ObjectDefaultEquals, ObjectDefaultCompare \
};

#define DEFINE_OBJECT_TYPE_ONLY(type) \
__declspec(dllexport) const char* type##_TYPE(){return #type;};

#define EXTENDS_OBJECT Object __obj

/*
It requires object type, which is can be defined by macro DEFINE_TYPE
*/
#define OBJECT_SUPER(type, ptr) \
InitObject(ptr, type##_TYPE, Object_##type##_InternalMethods);

typedef void* vtable_;
typedef const char* (*objectType) (); /* The object type is actually address of some global function */

typedef void* (*objectInternalMethod) (void* obj, ...);

/* GUIDE

0. Create Object Type:

	DEFINE_OBJ_TYPE(typename); // in .c file!

1. Create struct, which extends object:

	typedef struct SomeStruct{
		EXTENDS_OBJECT;
		...
	}

2. Init object struct:

	SomeStruct* NewSomeStruct(...)
	{
		SomeStruct* ptr = ...; //alloc
		OBJECT_SUPER(SomeStruct, ptr);
		...
	}

3. Override standart internal object methods:

	use macro, for exapmple override hash func:

	OverrideObjectHash(SomeStruct, func)

	in NewSomeStruct(...);

*/

typedef struct tagObject {

	pointer_value __addr; // should be integer value of pointer to self
	objectInternalMethod* internalMethods; // { free, hash, equals, compare }
	objectType type;

} Object;


typedef void* (*objectInteractFunc) (Object* obj);

static void ObjectDefaultDispose(Object* obj) {
	return;
}

static __int64 ObjectDefaultHash(Object* obj) {
	return (__int64)obj;
}

static __int64 ObjectDefaultEquals(Object* obj, Object* with) {
	return ((__int64)obj == (__int64)with);
}

static __int64 ObjectDefaultCompare(Object* obj, Object* with) {

	if (obj != 0 && with != 0) {
		return 0;
	}
	if (obj == 0 && with == 0) {
		return 0;
	}
	if (obj == 0) {
		return -1;
	}
	if (with == 0) {
		return 1;
	}
	return 0;
}

static objectInternalMethod ObjectInternalMethods[4] = {
	ObjectDefaultDispose, ObjectDefaultHash, ObjectDefaultEquals, ObjectDefaultCompare
};


typedef void* (*objectVirtualFunc) (void* obj, ...);

void InitObject(Object* obj, objectType type, objectInternalMethod* optTable);

int isObjectsEquals(Object* obj, Object* with);
__int64 getObjectHash(Object* obj);
int compareObjects(Object* obj, Object* with);

void DisposeObject(Object* obj);

/*Important! objPtr should be pointer to pointer*/
int DestroyObject(Object** objPtr);

void printObjectType(Object* obj);

#define OverrideObjectDispose(type, func) \
Object_##type##_InternalMethods[0] = func;

#define OverrideObjectHash(type, func) \
Object_##type##_InternalMethods[1] = func;

#define OverrideObjectEquals(type, func) \
Object_##type##_InternalMethods[2] = func;

#define OverrideObjectCompare(type, func) \
Object_##type##_InternalMethods[3] = func;

pointer_value doNothing(void* arg, ...);