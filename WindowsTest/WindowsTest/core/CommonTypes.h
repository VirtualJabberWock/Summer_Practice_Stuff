#pragma once
#include "Object.h"
#include "Functional.h"

typedef unsigned char byte_t;
typedef unsigned char bool_t;
typedef void* NumberModel;
typedef __int64 NumREGISTER;

#define True 1
#define False 0

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


/*
About String:
@param [char*] ptr - char array (content of string)
@param [size_t] len - length of char array
@param [bool] isIsntantFree - flag to instant free, when NewString() return value used as a argument
@warning for function [add] : NativeReflection Required!!!
*/

OBJECT_CLASS_FM(String,

	char* ptr;
	size_t len;  
	bool_t isInstantFree;
,
	struct String_mtable_tag* (*add)(const char* str);
	Object* (*split)(struct tagString* self, const char* pattern);
	int (*c_cmp)(struct tagString* self, const char* c_str);
	const char* (*copy) (struct tagString* self);
	void (*within)(struct tagString* self, char from, char to);
	int (*startsWith)(struct tagString* self, const char* pattern);
	//...

);


#define ERR_VERY_LONG_STRING 0x5709999 // 57 like ST, STRING ERROR

String* NewString(const char* base);
String* TempString(const char* base);
String* String_Free(String* self);
String* CastString(Object* obj);

int StringCompare(String* str, String* str2, __int64* opt_outHash);
__int64 StringHash(String* str);
__int64 Hash_C_String(const char* str);

const char* copy_c_string(const char* buffer, int max_len);
const char* copy_c_string_l(const char* buffer, size_t len);