#include "Functional.h"
#include <stdio.h>
#include <varargs.h>
#include <limits.h>
#include <ctype.h>
#include <Windows.h>
#include <winbase.h>

typedef const char* (*getObjectType)();

void _DebugObjectMethod(Object* obj, ...)
{
	va_list args;
	va_start(args, obj);
	getObjectType _getType = obj->type;
	printf("\n Object<%s> [%p] called some internal method:\n", _getType(), obj);
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
