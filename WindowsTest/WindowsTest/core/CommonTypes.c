#include "CommonTypes.h"
#include "DebugUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

String* add(String* p, const char* str) {
	int len = strlen(str);
	if (p == 0) throw NULL_POINTER_EXCEPTION;
	if (p->ptr == 0) throw NULL_POINTER_EXCEPTION;
	if (len + p->len > INT_MAX) {
		panic("Very long string", ERR_VERY_LONG_STRING);
		return 0;
	}
	int total = len + p->len;
	char* ptr_ = malloc(sizeof(char) * (total + 1));
	if (ptr_ == NULL) {
		panic("Can't alloc memory!", ERR_CANT_ALLOC_MEM);
		return 0;
	};
	ptr_[total] = '\0';
	for (int i = 0; i < p->len; i++) {
		ptr_[i] = p->ptr[i];
	}
	for (int i = p->len; i < total; i++) {
		ptr_[i] = str[i - p->len];
	}
	free(p->ptr);
	p->len = total;
	p->ptr = ptr_;
	return p;
}


struct String_mtable_tag String_METHODS[] = { { add } };

String* NewString(const char* base)
{
	String* s = malloc(sizeof(String));
	if (s == 0) return 0;
	OBJECT_SUPER_FM(String, s);
	s->len = strlen(base);
	s->ptr = malloc(sizeof(char) * (s->len + 1));
	if (s->ptr == 0) return 0;
	for (int i = 0; i < s->len; i++) {
		s->ptr[i] = base[i];
	}
	s->ptr[s->len] = '\0';
	s->free = String_Free;
	return s;
}

String* String_Free(String* self)
{
	if (checkObjectType(self, String_TYPE) == 0) return;
	if (self->ptr != 0) free(self->ptr);
	self->ptr = 0;
	self->len = 0;
}

String* CastString(Object* obj)
{
	if (checkObjectType(obj, String_TYPE) == 0) {
		throw INVALID_CAST_EXCEPTION("String");
	}
	return obj;
}
