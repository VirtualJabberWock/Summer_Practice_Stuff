#include "Vector.h"

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

struct ObjectVector_mtable_tag ObjectVector_METHODS[] = { { push, pop, get, clear } };

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
