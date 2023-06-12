#pragma once
#include "Stream.h"

STREAM_CLASS_FM(StreamStdOut,
	int tag0;
,
	struct tagStreamStdOut (*str) (struct tagStreamStdOut, const char* str);
	struct tagStreamStdOut (*num) (struct tagStreamStdOut, __int64 number);
	struct tagStreamStdOut (*numf) (struct tagStreamStdOut, double d);
)