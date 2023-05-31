#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "DebugUtil.h"
#include "Object.h"
#include "Vector.h"

#define ERR_KEY_DONT_EXISTS 0xd1c70001

OBJECT_CLASS_FM(HashMap,

	ObjectVector data;
	ObjectType common_type;
,
	void(*emplace) (struct tagHashMap* self, Object* key, Object* value);
	Object* (*get) (struct tagHashMap* self, Object* key);

);

HashMap* NewHashMap(ObjectType common_type);