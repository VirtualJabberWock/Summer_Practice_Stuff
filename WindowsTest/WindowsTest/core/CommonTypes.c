#include "CommonTypes.h"
#include "DebugUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Vector.h"

DEFINE_TYPE(String);

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

VectorString* String_split(String* str, const char* pattern) {

	if (str == 0) throw NULL_POINTER_EXCEPTION;
	if (str->ptr == 0) throw NULL_POINTER_EXCEPTION;

	VectorString* v = NewVectorT(4, String_TYPE);
	int p = 0; int p_l = strlen(pattern);
	int last_pos = 0;
	int div_pos = 0; char temp = 0;
	for (int i = 0; i < str->len; i++) {
		if (str->ptr[i] == pattern[p]) {
			p++;
			if (p == p_l - 1) {
				div_pos = i-p_l+2;
				temp = str->ptr[div_pos];
				str->ptr[div_pos] = 0;
				v->_->push(v,NewString(str->ptr + last_pos));
				str->ptr[div_pos] = temp;
				last_pos = i+2; p = 0;
			}
		}
		else p = 0;
	}
	if (str->len - last_pos <= 0) return v;
	v->_->push(v, NewString(str->ptr + last_pos));
	return v;
}

int String_c_cmp(String* str, const char* c_str) {

	if (c_str == 0 && str == 0) return 0;
	if (c_str == 0 && str != 0) return 1;
	if (c_str != 0 && str == 0) return -1;

	if (str->ptr == 0) throw NULL_POINTER_EXCEPTION;

	return (strcmp(str->ptr, c_str));
}

const char* String_copy(String* str) {

	if (str == 0) throw NULL_POINTER_EXCEPTION;
	if (str->ptr == 0) throw NULL_POINTER_EXCEPTION;

	return copy_c_string_l(str->ptr, str->len);
}

void String_within(String* str, char from, char to) {

	if (str == 0) throw NULL_POINTER_EXCEPTION;
	if (str->ptr == 0) throw NULL_POINTER_EXCEPTION;
	char* buffer = calloc(str->len + 1, sizeof(char));
	if (buffer == 0) throw MEM_PANIC_RETURN_V;
	int b_pos = 0; int is_ = 0;
	int b_len = str->len;
	for (int i = 0; i < str->len; i++) {
		if (is_) {
			if (str->ptr[i] == to) 
				break; b_len = i + 1;
			buffer[b_pos] = str->ptr[i];
			b_pos++;
		} else if (str->ptr[i] == from) is_ = 1;
	}
	free(str->ptr);
	str->ptr = copy_c_string_l(buffer, b_len);
	free(buffer);
}

int String_StartsWith(String* s, const char* p) {

	if (s == 0) throw NULL_POINTER_EXCEPTION;
	if (s->ptr == 0) throw NULL_POINTER_EXCEPTION;
	for (int i = 0; i < s->len + 1; i++) {
		if (p[i] == '\0') return 1;
		if (s->ptr[i] != p[i]) return 0;
	}
	return 0;
}


struct String_mtable_tag String_METHODS[] = { { 
		add, String_split, String_c_cmp, String_copy, String_within, String_StartsWith
} };

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
	s->isInstantFree= 0;
	s->free = String_Free;
	s->compare = StringCompare;
	return s;
}

String* TempString(const char* base)
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
	s->isInstantFree = 1;
	s->free = String_Free;
	s->compare = StringCompare;
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
	if (obj == 0) throw NULL_POINTER_EXCEPTION;
	if (checkObjectType(obj, String_TYPE) == 0) {
		throw INVALID_CAST_EXCEPTION("String");
	}
	return obj;
}

int StringCompare(String* str, String* str2, __int64* opt_outHash)
{
	if (str == 0 && str2 == 0) return 0;
	if (str == 0 && str2 != 0) return -1;
	if (opt_outHash != 0) *opt_outHash = StringHash(str);
	if (str != 0 && str2 == 0) return 1;
	if (!isObject(str)) {
		return 0;
	}
	if (str->__type != String_TYPE) return standartCompare(str, str2, opt_outHash);
	if (str2->__type != String_TYPE) return standartCompare(str, str2, opt_outHash);
	return strcmp(str->ptr, str2->ptr);
}

__int64 StringHash(String* str)
{
	if (str == 0) return 0;
	if (str->__type != String_TYPE) return (__int64)str;
	__int64 h = 1;
	for (int i = 0; i < str->len; i++) {
		h = 31 * h + str->ptr[i];
	}
	return h;
}

__int64 Hash_C_String(const char* str)
{
	if (str == 0) return 0;
	__int64 h = 1;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		h = 31 * h + str[i];
	}
	return h;
}

const char* copy_c_string(const char* buffer, int max_len)
{
	int i = 0;
	while (buffer[i] != 0) {
		i++; if (i > max_len) break;
	}
	return copy_c_string_l(buffer, i);
}

const char* copy_c_string_l(const char* buffer, size_t len)
{
	char* new_str = malloc((len + 1) * sizeof(char));
	if (new_str == 0)
		throw MEM_PANIC_RETURN_0;
	for (int i = 0; i < len; i++) {
		new_str[i] = buffer[i];
	}
	new_str[len] = '\0';
	return new_str;
}
