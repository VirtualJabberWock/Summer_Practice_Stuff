#include "DebugUtil.h"
#include <stdio.h>
#include <varargs.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>

#ifdef _WIN32
#include <Windows.h>
#include <winbase.h>
#endif

typedef const char* (*getObjectType)();

void _DebugObjectMethod(Object* obj, ...)
{
	va_list args;
	va_start(args, obj);
	getObjectType _getType = obj->type;
	if (isObject(obj)) {
		printf("\n Object<%s> [%p] called some internal method:\n", _getType(), obj);
	}
	printf("\n\tWITH ARGS(+STACK):  {");
	unsigned __int64 ptr = 1;
	while (ptr != 0 && ptr != 0xcccccccccccccccc) {
		ptr = va_arg(args, __int64);
		if (ptr == 0xcccccccccccccccc) break;
		if (ptr < INT_MAX) {
			printf("%p(%lld)", ptr, ptr);
		}
		else {
			printf("%p", ptr);
		}
		printf(", ");
	}
	printf("...} \n");
	printf("\n");
	va_end(obj);
}

int panic(const char* err_msg, unsigned int err_code)
{
	if (exit_if_error) {
#ifdef _WIN32
		return win_panic(err_msg);
#else
		return unix_panic(err_msg);
#endif
	}
	else {
		int len = strlen(err_msg);
		last_error = malloc(sizeof(char) * (len + 1));
		last_error_code = err_code;
		if (last_error == 0) return win_panic("Can't alloc memory! So bad!");
		for (int i = 0; i < len; i++) {
			last_error[i] = err_msg[i];
		}
		last_error[len] = '\0';
		last_check = 1;
	}
}

int panic_npe(const char* func_name, int line, const char* filename)
{
	char* buffer[1000];
	sprintf_s(buffer, 1000, "Null pointer exception, at line %d in function : %s\nFile: %s", line, func_name, filename);
	return panic(buffer, ERR_NULL_POINTER);
}

int panic_cast(const char* to_type, const char* func_name, int line, const char* filename)
{
	char* buffer[1000];
	sprintf_s(buffer, 1000, "Invalid cast to <%s>, at line %d in function : %s\nFile: %s", to_type, line, func_name, filename);
	return panic(buffer, ERR_INVALID_CAST);
}

int win_panic(const char* err_msg)
{
#ifdef _WIN32
	MessageBoxA(0, err_msg, "Fatal Error! (panic!)", 0x40L);
	exit(-1);
	return 0;
#endif
}

int unix_panic(const char* err_msg)
{
	return 0;
}

int TryToExecute(OnException optional, VoidWrap action, void* arg0)
{
	last_check = 0;
	exit_if_error = 0;
    action(arg0);
	exit_if_error = 1;
	if (last_check == 1) {
		if (last_error != NULL) {
			int z = 0;
			if (optional != NULL) z = optional(last_error, last_error_code);
			free(last_error);
			last_error = 0;
			return (z == 0);
		}
		else {
			if (optional != NULL) optional("UNKNOWN EXCEPTION (can't read last_error)", -1);
			return 1;
		}
	}
	return 0;
}

const char* TryToExecuteSaveError(VoidWrap action, void* arg0)
{
	last_check = 0;
	exit_if_error = 0;
	action(arg0);
	exit_if_error = 1;
	if (last_check == 1) {
		if (last_error != NULL) {
			return last_error;
		}
		else {
			win_panic("WTF!");
			return 0;
		}
	}
	return 0;
}
