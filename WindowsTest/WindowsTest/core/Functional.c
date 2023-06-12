#include "Functional.h"
#include "DebugUtil.h"
#include <stdlib.h>

__int64 DoNothing(void* arg0, ...) {
	return 0;
}

void NotImplemented(void* arg0, ...)
{
	panic("Try to call not implemented func!", ERR_NOT_IMPLEMENTED);
}


