#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "DebugUtil.h"
#include "Object.h"
#include "Vector.h"
#include "List.h"

#define ERR_KEY_DONT_EXISTS 0xd1c70001

OBJECT_CLASS_FM(HashMap,

	ObjectVector* data;
	ObjectVector* keys;
	__int64 static_size;
	int save_keys;
,
	void(*emplace) (struct tagHashMap* self, Object* key, Object* value);
	void(*emplace_by_str) (struct tagHashMap* self, const char* key, Object* value);
	ObjectNode* (*emplace_by_hash) (struct tagHashMap* self, __int64 hash, Object* value);
	Object* (*get) (struct tagHashMap* self, Object* key);
	Object* (*gets) (struct tagHashMap* self, const char* key);
	Object* (*getByHash) (struct tagHashMap* self, __int64 hash);

);

// Dictionary, also should keep all keys
typedef HashMap Dictionary;

HashMap* NewHashMap();
Dictionary* NewDictionary();
void  FreeHashMap(HashMap* hashmap);