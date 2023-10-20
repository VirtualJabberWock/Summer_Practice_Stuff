#include "String.h"

#include <string.h>
#include <memory.h>

DEFINE_OBJ_TYPE(String);

static __int64 getStringHash(String* str) {
	if (str->__obj.type != TYPE_String) {
		return (__int64)str;
	}
	__int64 hash = str->length;
	for (int i = 0; i < str->length; i++) {
		hash ^= (str->data[i]) << ((i) % 64);
	}
	return hash;
}

static void freeString(String* str) {
	if (str->__obj.type != TYPE_String) {
		return;
	}
	if (str->data != 0) free(str->data);
	if (str->__wide_str != 0) free(str->__wide_str);
	str->data = 0;
	str->__wide_str = 0;
	str->length = 0;
};

static int compareString(String* str, String* with) {

	if (str->__obj.type != TYPE_String 
		|| with->__obj.type != TYPE_String) {
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

	if (str->data == 0) return debugMemError();

	memcpy(str->data, base, str->length);

	str->__wide_str = calloc(str->length + 1, sizeof(wchar_t));
	if (str->__wide_str == 0) return debugMemError();
	for (int i = 0; i < str->length + 1; i++) {
		str->__wide_str[i] = str->data[i];
	}

	return str;
}

#include <stdio.h>
#include <varargs.h>

String* NewStringFormat(const char* format, ...)
{

	va_list args;
	va_start(args, format);
	const char* fmt = va_arg(args, const char*);

	char buffer[2000];
	int length = strlen(format);
	vsprintf_s(buffer, 2000, format, args);
	
	return NewString(buffer);
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

char* StringToChars(String* str)
{
	char* array = calloc(str->length + 1, sizeof(char));
	if (array == 0) return debugMemError();
	memcpy(array, str->data, str->length);
	array[str->length] = 0;
	return array;
}

int isStringTemp(Object* obj)
{
	if (obj->type != TYPE_String) {
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
	if (obj->type != TYPE_String) {
		return 0;
	}
	return obj;
}

