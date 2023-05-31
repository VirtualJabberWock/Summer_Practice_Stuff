#pragma once
#include "Object.h"

typedef union _tag_LongNumber {
	double d;
	__int64 n;
	char bytes[8];
	__int32 HL[2]; // HIGH LOW
} LongNumber;

typedef union _tag_Number {
	float f;
	__int32 n;
	char bytes[4];
	__int16 HL[2]; // HIGH LOW
} Number;

typedef struct tag_c_string {
	const char* ptr;
	int len;
} c_string;


OBJECT_CLASS_FM(String,

	char* ptr;
	int len;
	int isInstantFree;
,
	struct tagString* (*add)(struct tagString* self, const char* str);
	//...

)


#define ERR_VERY_LONG_STRING 0x5709999 // 57 like ST, STRING ERROR

String* NewString(const char* base);
String* TempString(const char* base);
String* String_Free(String* self);
String* CastString(Object* obj);

int StringCompare(String* str, String* str2, __int64* opt_outHash);
__int64 StringHash(String* str);
__int64 Hash_C_String(const char* str);