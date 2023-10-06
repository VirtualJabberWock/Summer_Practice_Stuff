#pragma once

#define DEBUG_MODE 1

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#include <stdlib.h>

void debugShowWarning(const char* msg);
int debugFatalError(const char* msg);
int debugFatalErrorFormat(const char* format, ...);

int debugMemError();

int isExecuteable(void* ptr);