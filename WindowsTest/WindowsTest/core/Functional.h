#pragma once
#include "Object.h"

typedef void (*OnResponse)(int status, const char* data);
typedef void (*OnAction)();
typedef void (*VoidWrap) (void* arg0, ...);

typedef __int64 (*func_prototype64) (void* arg0, ...);
typedef __int32 (*func_prototype86) (void* arg0, ...);

__int64 DoNothing(void* arg0, ...);
void NotImplemented(void* arg0, ...);


#if _WIN32 || _WIN64
#if _WIN64
#define func_prototype func_prototype64
#else
#define func_prototype func_prototype86
#endif
#endif
