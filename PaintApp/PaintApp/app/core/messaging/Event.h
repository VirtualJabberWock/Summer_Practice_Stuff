#pragma once

#include "..\Object.h"
#include <Windows.h>

typedef void* (*OnEventFunc)(Object* optSubscriber, struct tagEvent* event);

typedef __int64 EventUUID; /* actually this is just value of Object_TYPE*/

EventUUID __Event_getUUID(const char* name);

/*This is not very fast way to get EventUUID, don't use this in loops*/
#define GetEventUUID(EventType) __Event_getUUID("TYPE_" ## #EventType)

typedef struct tagEvent {

	EXTENDS_OBJECT;
	Object* optSender;

} Event;

#define EXTENDS_EVENT Event __meta

Event* __CreateCommonEvent(objectType eventType, int sizeOfType);

#define CreateCommonEvent(type) __CreateCommonEvent(TYPE_##type, sizeof(type));