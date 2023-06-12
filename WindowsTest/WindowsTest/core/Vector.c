#include "Vector.h"

DEFINE_TYPE(ObjectVector);
DEFINE_TYPE(NumberVector);

void push(ObjectVector* self, Object* obj){

	if (self == NULL) {
		if (obj != 0) {
			if (obj->free != 0) obj->free(obj); free(obj);
		}
		throw NULL_POINTER_EXCEPTION;
	}

	if (self->__common_type != 0) {
		if (obj->type != self->__common_type) {
			if (obj != 0) {
				if (obj->free != 0) obj->free(obj); free(obj);
			}
			char buffer[800];
			ObjectType getType = (ObjectType)self->__common_type;
			sprintf_s(buffer, 800, "Trying put uncommon type, to Vector<%s>...", getType());
			return panic(buffer, ERR_NOT_COMMON_TYPE);
		}
	}

	if (self->size + 1 > self->capacity) {
		self->capacity = self->capacity * 2;
		Object** ptr_ = (Object**)realloc(self->ptr, sizeof(Object*)*self->capacity);
		if (ptr_ == NULL) throw MEM_PANIC_RETURN_V;
		self->ptr = ptr_;
	}

	self->ptr[self->size] = obj;
	self->size += 1;
}

Object* pop(ObjectVector* self){

	if (self == NULL) throw NULL_POINTER_EXCEPTION;

	Object* ret = self->ptr[self->size - 1];
	self->size -= 1;
	if (self->size < 0) self->size = 0;
	return ret;
}

Object* get(ObjectVector* self, int id)
{

	if (self == NULL) throw NULL_POINTER_EXCEPTION;

	if (id < 0 || id >= self->size) {
		panic("Index is outside the bounds of the array!", ERR_INDEX_OUT_OF_BOUNDS);
		return 0;
	}

	return self->ptr[id];

}

void clear(ObjectVector* self) {

	if (self == NULL) throw NULL_POINTER_EXCEPTION;
	if (self->ptr == NULL) return;

	for (int i = 0; i < self->size; i++) {
		Object* obj = self->ptr[i];
		if (obj != 0) {
			if (isObject(obj)) obj->free(obj);
			free(obj);
		}
		self->ptr[i] = 0;
	}

	free(self->ptr);
	self->capacity = 2;
	self->size = 0;

	self->ptr = (Object**)calloc(self->capacity, sizeof(Object*));
	if (self->ptr == NULL) throw MEM_PANIC_RETURN_V;
}

void NumVector_push(NumberVector* self, NumREGISTER num) {

	if (self == NULL) {
		throw NULL_POINTER_EXCEPTION;
	}

	LongNumber* p0 = (self->numType == NV_64) ? self->bucket[NV_64] : 0;
	Number* p1 = (self->numType == NV_32) ? self->bucket[NV_32] : 0;
	byte_t* p2 = (self->numType == NV_BYTE) ? self->bucket[NV_BYTE] : 0;

	if (self->size + 1 > self->capacity) {
		self->capacity = self->capacity * 2;
		if (self->numType == NV_64) {
			p0 = (LongNumber*)realloc(self->bucket[NV_64], sizeof(LongNumber) * self->capacity);
			if (p0 == NULL) throw MEM_PANIC_RETURN_V;
			self->bucket[NV_64] = p0;

		}
		if (self->numType == NV_32) {
			p1 = (Number*)realloc(self->bucket[NV_32], sizeof(Number) * self->capacity);
			if (p1 == NULL) throw MEM_PANIC_RETURN_V;
			self->bucket[NV_32] = p1;

		}
		if (self->numType == NV_BYTE) {
			p2 = (byte_t*)realloc(self->bucket[NV_BYTE], sizeof(byte_t) * self->capacity);
			if (p2 == NULL) throw MEM_PANIC_RETURN_V;
			self->bucket[NV_BYTE] = p2;
		}
	}

	if (self->numType == NV_64 && p0 != 0) p0[self->size].n = num;
	if (self->numType == NV_32 && p1 != 0) p1[self->size].n = num;
	if (self->numType == NV_BYTE && p2 != 0) p2[self->size] = (byte_t) num;

	self->size += 1;
}

NumREGISTER NumVector_pop(NumberVector* self){

	if (self == NULL) throw NULL_POINTER_EXCEPTION;

	NumREGISTER ret = 0;

	if (self->numType == NV_64) {
		ret = ((LongNumber*)self->bucket[NV_64])[self->size - 1].n;
	}
	if (self->numType == NV_32) {
		ret = ((Number*)self->bucket[NV_32])[self->size - 1].n;
	}
	if (self->numType == NV_BYTE) {
		ret = ((byte_t*)self->bucket[NV_BYTE])[self->size - 1];
	}
	self->size -= 1;
	if (self->size < 0) self->size = 0;
	return ret;
}

NumREGISTER NumVector_get(NumberVector* self, int id)
{

	if (self == NULL) throw NULL_POINTER_EXCEPTION;

	if (id < 0 || id >= self->size) {
		panic("Index is outside the bounds of the array!", ERR_INDEX_OUT_OF_BOUNDS);
		return 0;
	}

	if (self->numType == NV_64) {
		LongNumber* ln = self->bucket[NV_64]; return ln[id].n;
	}
	if (self->numType == NV_32) {
		Number* ln = self->bucket[NV_32]; return ln[id].n;
	}
	if (self->numType == NV_BYTE) {
		byte_t* ln = self->bucket[NV_BYTE]; return ln[id];
	}

	return 0;
}

void NumVector_clear(NumberVector* self) {

	if (self == NULL) throw NULL_POINTER_EXCEPTION;
	if (self->bucket == NULL) return;

	free(self->bucket);
	self->capacity = 2;
	self->size = 0;

	self->bucket = (NumberModel)calloc(3, sizeof(NumberModel));
	int a[3] = { 0 };
	a[self->numType] = self->capacity;
	self->bucket[NV_BYTE] = (byte_t*)calloc(a[NV_BYTE], sizeof(byte_t));
	self->bucket[NV_32] = (Number*)calloc(a[NV_32], sizeof(Number));
	self->bucket[NV_64] = (LongNumber*)calloc(a[NV_64], sizeof(LongNumber));

	if (self->bucket == NULL) throw MEM_PANIC_RETURN_V;
}

struct ObjectVector_mtable_tag ObjectVector_METHODS[] = { { push, pop, get, clear } };
struct NumberVector_mtable_tag NumberVector_METHODS[] = { { NumVector_push, NumVector_pop, NumVector_get, NumVector_clear } };

ObjectVector* NewObjectVector(int pre_capacity)
{

	ObjectVector* obj_v = malloc(sizeof(ObjectVector));
	if (obj_v == 0) throw MEM_PANIC_RETURN_0;
	OBJECT_SUPER_FM(ObjectVector, obj_v);
	obj_v->capacity = pre_capacity;
	obj_v->size = 0;
	if (pre_capacity < 1) pre_capacity = 1;
	if (pre_capacity > INT_MAX/2) pre_capacity = 1073741824;
	obj_v->ptr = (Object**)calloc(pre_capacity, sizeof(Object*));

	if (obj_v->ptr == 0) {
		free(obj_v);
		throw MEM_PANIC_RETURN_0;
	}

	obj_v->__common_type = 0;
	obj_v->free = FreeObjectVector;
	return obj_v;

}

NumberVector* NewNumberVector(int pre_capacity, NumVectorType type)
{
	NumberVector* num_v = malloc(sizeof(NumberVector));
	if (num_v == 0) throw MEM_PANIC_RETURN_0;
	OBJECT_SUPER_FM(NumberVector, num_v);
	num_v->capacity = pre_capacity;
	num_v->size = 0;
	if (pre_capacity < 1) pre_capacity = 1;
	if (pre_capacity > INT_MAX / 2) pre_capacity = 1073741824;
	num_v->bucket = (NumberModel)calloc(3, sizeof(NumberModel));
	int a[3] = { 0 };
	a[type] = pre_capacity;
	num_v->bucket[NV_BYTE] = (byte_t*)calloc(a[NV_BYTE], sizeof(byte_t));
	num_v->bucket[NV_32] = (Number*)calloc(a[NV_32], sizeof(Number));
	num_v->bucket[NV_64] = (LongNumber*)calloc(a[NV_64], sizeof(LongNumber));

	if (num_v->bucket == 0) {
		free(num_v);
		throw MEM_PANIC_RETURN_0;
	}

	num_v->numType = type;
	num_v->free = FreeNumberVector;
	return num_v;
}

VectorT* NewVectorT(int pre_capacity, ObjectType main_type)
{
	ObjectVector* obj_v = NewObjectVector(pre_capacity);
	obj_v->__common_type = (__int64)main_type;
}

void FreeObjectVector(ObjectVector* self)
{
	if (checkObjectType(self, ObjectVector_TYPE) == 0) {
		return;
	}

	if (self->ptr == NULL) return;

	for (int i = 0; i < self->size; i++) {
		Object* obj = self->ptr[i];
		if (obj != 0) {
			if (isObject(obj) && obj->free != 0) obj->free(obj);
			free(obj);
		}
		self->ptr[i] = 0;
	}

	free(self->ptr);
	self->capacity = 2;
	self->size = 0;
	self->ptr = NULL;
}

void FreeNumberVector(NumberVector* self)
{
	if (checkObjectType(self, NumberVector_TYPE) == 0) {
		return;
	}
	if (self->bucket == NULL) return;
	free(self->bucket[NV_BYTE]);
	free(self->bucket[NV_32]);
	free(self->bucket[NV_64]);
	free(self->bucket);
	self->capacity = 2;
	self->size = 0;
	self->bucket = NULL;
}
