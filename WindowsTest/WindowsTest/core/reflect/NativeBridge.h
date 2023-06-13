#pragma once

#include <intrin.h>
#include <stdlib.h>
#include "..\Functional.h"

#if _WIN32 || _WIN64

#include <Windows.h>
#if _WIN64
typedef __int64 ret_register;
#else
typedef __int32 ret_register;
#endif

#elif __linux__

typedef __int64 ret_register;

#endif

#define GetCallerAdress() (__int64)_ReturnAddress()

#ifdef _WIN32
	static NoArgsFunction bakeCustomFunction(char* buffer, int len){
		char* cave = calloc(len, sizeof(char));
		for (int i = 0; i < len; i++) cave[i] = buffer[i];
		DWORD old = 0;
		VirtualProtect(cave, len*2, PAGE_EXECUTE_READWRITE, &old);
		NoArgsFunction f = (NoArgsFunction) cave;
		return f;
	}
#elif __linux__
static NoArgsFunction bakeCustomFunction(...) {
	return; //TODO
};
#endif

__int64 InitNativeReflection();

#define IMPORT_NATIVE_REFLECTION \
extern int isNativeReflectionInitialized; \
extern NoArgsFunction getStackRBP; \
//...

#define GET_CALLER_OBJECT(OBJ_TYPE) \
if (getStackRBP == 0) return panic("Native Reflection Required!\n(call InitNativeReflection in main func)", 0x500); \
unsigned char* __caller_asm1 = (GetCallerAdress() - 21); \
int __caller_offset = -686868; \
for (int i = 0; i < 16; i++) { \
	if ( \
		(__caller_asm1[i] == 0x48) && \
		(__caller_asm1[i + 1] == 0x8b) && \
		(__caller_asm1[i + 2] == 0x45) \
		) {__caller_offset = __caller_asm1[i + 3] / sizeof(ret_register); break; } \
} \
if (__caller_offset == -686868) return; \
__int64* __caller_stack = (getStackRBP() + 448); \
int _caller_z_offset = __caller_stack[0]; \
OBJ_TYPE* __self__ = __caller_stack[__caller_offset];