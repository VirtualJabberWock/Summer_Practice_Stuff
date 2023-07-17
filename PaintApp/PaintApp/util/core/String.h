#pragma once

#include "Object.h"

typedef struct tagString { 

	EXTENDS_OBJECT;

	char* data;
	int length;
	char isTemporary; /*reserved: */

} String, TempString;

String* NewString(const char* base);
String* NewStringFormat(const char* format, ...);
TempString* NewTempString(const char* base);
String* NewStringFromArray(char* dynamicCharArray, int len);

int isStringTemp(Object* obj);
String* CastToString(Object* obj);