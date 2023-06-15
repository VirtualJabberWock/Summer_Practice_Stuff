#pragma once
#include <stdlib.h>
#include <stdio.h>

#include "DebugUtil.h"
#include "Object.h"
#include "CommonTypes.h"

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

typedef enum enumNumberVType {
	NV_BYTE = 0,
	NV_32 = 1,
	NV_INT = 1,
	NV_64 = 2,
	NV_LONG = 2
} NumVectorType;

OBJECT_CLASS_FM(NumberVector,

	NumberModel* bucket;
	int size;
	int capacity;
	__int64 numType;

,
	void (*push) (struct tagNumberVector* self, NumREGISTER num);
	NumREGISTER (*pop) (struct tagNumberVector* self);
	NumREGISTER (*get) (struct tagNumberVector* self, int id);
	void (*clear) (struct tagNumberVector* self);
	struct NumberVector_mtable_tag* (*set) (struct tagNumberVector* self, int id, NumREGISTER value);

);

typedef ObjectVector VectorT;
typedef ObjectVector VectorString;

ObjectVector* NewObjectVector(int pre_capacity);
NumberVector* NewNumberVector(int pre_capacity, NumVectorType type);
VectorT* NewVectorT(int pre_capacity, ObjectType main_type);
void FreeObjectVector(ObjectVector* obj_v);
void FreeNumberVector(NumberVector* obj_v);

typedef struct tagNativeVector {
	unsigned int* data;
	size_t size;
	size_t cap;
} NativeVector;

NativeVector* NewNativeVector();
void NativeVector_push(NativeVector* vec, int value);