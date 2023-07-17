#include "DebugUtil.h"
#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif

void debugShowWarning(const char* msg)
{
#ifdef _WIN32
	MessageBoxA(0, msg, "Warning!", MB_ICONWARNING);
#endif
}

int debugFatalError(const char* msg)
{
#ifdef _WIN32
	MessageBoxA(0, msg, "Fatal Error!", MB_ICONERROR);
#endif
	exit(-1);
	return 0;
}

#include <varargs.h>
#include <stdio.h>

int debugFatalErrorFormat(const char* format, ...)
{
#ifdef _WIN32
	va_list args;
	va_start(args, format);

	char* buffer = calloc(5000, sizeof(char));
	if (buffer == 0) {
		return -1;
	}
	vsprintf_s(buffer, 5000, format, args);
	MessageBoxA(0, buffer, "Fatal error!", 0x40L);
	free(buffer);
#endif
	exit(-1);
	return 0;
}

int debugMemError()
{
	return debugFatalError("Memory allocation error! exit...");
}
