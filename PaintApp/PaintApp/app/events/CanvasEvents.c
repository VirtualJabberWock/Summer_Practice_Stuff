#include "CanvasEvents.h"

DEFINE_OBJECT_TYPE_ONLY(CanvasInvalidateEvent);
DEFINE_OBJECT_TYPE_ONLY(ApplyHDCEvent);


Event* NewCanvasInvalidateEvent()
{
	return CreateCommonEvent(CanvasInvalidateEvent);
}

ApplyHDCEvent* NewApplyHDCEvent()
{
	return CreateCommonEvent(ApplyHDCEvent);
}