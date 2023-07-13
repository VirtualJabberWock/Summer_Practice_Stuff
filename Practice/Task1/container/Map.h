#pragma once

#include "..\core\Object.h"

typedef struct tagIMapVTABLE {

	void (*emplace)(Object* map, Object* key, Object* value);
	Object* (*get)(Object* map, Object* key);

} IMapVTABLE;

typedef struct  tagIMap {

	EXTENDS_OBJECT;

	IMapVTABLE* mapMethods;
	objectType mapType;

} IMap;

void mapEmplace(IMap* map, Object* key, Object* value);
Object* mapGet(IMap* map, Object* key);
Object* mapGetByString(IMap* map, const char* key);
Object* mapGetByInteger(IMap* map, int key);

void InitMap(IMap* map, objectType mapType, objectInternalMethod* mapMethods, objectInternalMethod* objectMethods);

#define EXTENDS_MAP IMap __map

#define MAP_SUPER(type, ptr) \
InitMap(ptr, type##_TYPE, IMap_##type##_InternalMethods, Object_##type##_InternalMethods);

#define DEFINE_MAP_TYPE(type) \
__declspec(dllexport) const char* type##_TYPE(){return #type;}; \
static objectInternalMethod Object_##type##_InternalMethods[4] = { \
	ObjectDefaultDispose, ObjectDefaultHash, ObjectDefaultEquals, ObjectDefaultCompare \
}; \
static objectInternalMethod IMap_##type##_InternalMethods[2] = { \
	doNothing, doNothing \
};

#define OverrideIMapEmplace(type, func) \
IMap_##type##_InternalMethods[0] = func;

#define OverrideIMapGet(type, func) \
IMap_##type##_InternalMethods[1] = func;

