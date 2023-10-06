#include "CanvasEvents.h"

DEFINE_OBJ_TYPE(CanvasInvalidateEvent);

Event* NewCanvasInvalidateEvent()
{
	return CreateCommonEvent(CanvasInvalidateEvent);
}
