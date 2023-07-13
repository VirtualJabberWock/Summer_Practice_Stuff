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
