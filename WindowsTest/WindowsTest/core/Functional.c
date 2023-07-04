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

NativeAnonymousContext* Functional_getNativeContext(AnonymousContext* context)
{
	if (context->type == 0) return context;
	else throw INVALID_CAST_EXCEPTION("NativeAnonymousContext");
}

Object* Functional_getMappedContext(AnonymousContext* context)
{
	MappedAnonymousContext* t;
	if (context->type == 1) t = context;
	else throw INVALID_CAST_EXCEPTION("MappedAnonymousContext");
	return t;
}

SingleAnonymousContext* Functional_getSingleObjectContext(AnonymousContext* context)
{
	if (context->type == 2) return context;
	else throw INVALID_CAST_EXCEPTION("SingleAnonymousContext");
}


