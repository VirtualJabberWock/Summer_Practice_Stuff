#include "String.h"

#include <string.h>
#include <memory.h>

DEFINE_OBJ_TYPE(String);

static __int64 getStringHash(String* str) {
	if (str->__obj.type != String_TYPE) {
		return (__int64)str;
	}
	__int64 hash = str->length;
	for (int i = 0; i < str->length; i++) {
		hash ^= (str->data[i]) << ((i*2) % 64);
	}
	return hash;
}

static void freeString(String* str) {
	if (str->__obj.type != String_TYPE) {
		return;
	}
	if (str->data != 0) free(str->data);
	str->data = 0;
	str->length = 0;
};

static int compareString(String* str, String* with) {

	if (str->__obj.type != String_TYPE 
		|| with->__obj.type != String_TYPE) {
		return 0;
	}

	if (str->data == 0 && with->data != 0) {
		return -1;
	}
	if (str->data != 0 && with->data == 0) {
		return 1;
	}
	if (str->data == 0 && with->data == 0) {
		return 0;
	}

	return strcmp(str->data, with->data);

}

String* NewString(const char* base)
{
	String* str = calloc(1, sizeof(String));

	if (str == 0) {
		return 0;
	}

	OBJECT_SUPER(String, str);
	OverrideObjectHash(String, getStringHash);
	OverrideObjectDispose(String, freeString);
	OverrideObjectCompare(String, compareString);

	str->length = strlen(base);
	str->data = calloc(str->length + 1, sizeof(char));
	str->isTemporary = 0;
	memcpy(str->data, base, str->length);

	return str;
}

TempString* NewTempString(const char* base)
{
	String* str = NewString(base);
	str->isTemporary = 1;
	return str;
}

String* NewStringFromArray(char* dynamicCharArray, int len)
{
	String* str = calloc(1, sizeof(String));

	if (str == 0) {
		return 0;
	}

	OBJECT_SUPER(String, str);
	OverrideObjectHash(String, getStringHash);
	OverrideObjectDispose(String, freeString);
	OverrideObjectCompare(String, compareString);

	str->data = dynamicCharArray;
	str->length = len;
	return str;
}

int isStringTemp(Object* obj)
{
	if (obj->type != String_TYPE) {
		return 0;
	}
	else {
		String* str = obj;
		return str->isTemporary;
	}
	return 0;
}

String* CastToString(Object* obj)
{
	if (obj == 0) return 0;
	if (obj->type != String_TYPE) {
		return 0;
	}
	return obj;
}

