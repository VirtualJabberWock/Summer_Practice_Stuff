#pragma once
#include "Stream.h"
#include "..\Functional.h"


STREAM_CLASS_FM(StreamStdOut,

	int tag0;
,

	struct StreamStdOut_mtable_tag* (*str) (const char* str);
	struct StreamStdOut_mtable_tag* (*num) (__int64 number);
	struct StreamStdOut_mtable_tag* (*numf) (double d);

);

StreamStdOut* NewSTDOutStream();