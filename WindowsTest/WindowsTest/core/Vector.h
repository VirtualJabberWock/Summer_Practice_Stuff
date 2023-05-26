#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "DebugUtil.h"
#include "Object.h"

#define ERR_INDEX_OUT_OF_BOUNDS 0x08001001
#define ERR_NOT_COMMON_TYPE 0x08001002

OBJECT_CLASS_FM(ObjectVector,

	Object** ptr;
	int size;
	int capacity;
	__int64 __common_type;

,
	void (*push) (struct tagObjectVector* self, Object* obj);
	Object* (*pop) (struct tagObjectVector* self);
	Object* (*get) (struct tagObjectVector* self, int id);
	void (*clear) (struct tagObjectVector* self);

);

typedef ObjectVector VectorT;

ObjectVector* NewObjectVector(int pre_capacity);
VectorT* NewVectorT(int pre_capacity, ObjectType main_type);
void FreeObjectVector(ObjectVector* obj_v);