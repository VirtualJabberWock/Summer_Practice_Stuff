#pragma once

#include "../core/messaging/Event.h"

typedef Event CanvasInvalidateEvent;

Event* NewCanvasInvalidateEvent();

typedef struct tagApplyHDCEvent 
{
	EXTENDS_EVENT;
	int x;
	int y;
	int width;
	int height;

} ApplyHDCEvent;

ApplyHDCEvent* NewApplyHDCEvent();