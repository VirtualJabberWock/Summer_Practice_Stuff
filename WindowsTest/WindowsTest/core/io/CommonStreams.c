#include "CommonStreams.h"
#include "..\DebugUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "..\reflect\NativeBridge.h"

IMPORT_NATIVE_REFLECTION;

DEFINE_TYPE(StreamStdOut);

static struct StreamStdOut_mtable_tag* MethodsInstance = 0;

struct StreamStdOut_mtable_tag* StreamStdOut_str (const char* str) {
	int len = strlen(str);
	fwrite(str, sizeof(char), len, stdout);
	return MethodsInstance;
}

struct StreamStdOut_mtable_tag* StreamStdOut_num (__int64 number) {
	fprintf_s(stdout, "%lld", number);
	return MethodsInstance;
}

struct StreamStdOut_mtable_tag* StreamStdOut_numf (double d) {
	fprintf_s(stdout, "%lf", d);
	return MethodsInstance;
}

struct StreamStdOut_mtable_tag StreamStdOut_METHODS[] = {
	{
		StreamStdOut_str,
		StreamStdOut_num,
		StreamStdOut_numf
	}
};


StreamStdOut* NewSTDOutStream()
{
	if(MethodsInstance == 0) MethodsInstance = StreamStdOut_METHODS;
    StreamStdOut* s = calloc(1, sizeof(StreamStdOut));
    if (s == 0) throw MEM_PANIC_EXCEPTION;
    STREAM_SUPER_FM(StreamStdOut, s);
	return s;
}
