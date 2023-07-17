#include "Map.h"
#pragma once

DEFINE_OBJECT_TYPE_ONLY(IMap); 

#include "..\core\String.h"
#include "..\core\CommonTypes.h"

void mapEmplace(IMap* map, Object* key, Object* value)
{
	map->mapMethods->emplace(map, key, value);
}

Object* mapGet(IMap* map, Object* key)
{
	return map->mapMethods->get(map, key);
}

Object* mapGetByString(IMap* map, const char* key)
{
	String* str = NewString(key);
	Object* result = map->mapMethods->get(map, str);
	DestroyObject(&str);
	return result;
}

Object* mapGetByInteger(IMap* map, int key)
{
	Integer* number = NewInteger(key);
	Object* result = map->mapMethods->get(map, number);
	DestroyObject(&number);
	return result;
}

void InitMap(IMap* map, objectType mapType, objectInternalMethod* mapMethods, objectInternalMethod* objectMethods)
{
	InitObject(map, IMap_TYPE, objectMethods);
	map->mapType = mapType;
	map->mapMethods = mapMethods;
}
