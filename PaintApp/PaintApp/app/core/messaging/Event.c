#include "Event.h"
#pragma once

DEFINE_OBJ_TYPE(Event);

EventUUID __Event_getUUID(const char* name)
{
	return (EventUUID) GetProcAddress(GetModuleHandle(0), name);
}

Event* __CreateCommonEvent(objectType eventType, int size)
{
	Event* event = calloc(1, size);
	OBJECT_SUPER(Event, event);
	if (event == 0) {
		return debugMemError();
	}
	event->optSender = 0;
	event->__obj.type = eventType;
	return event;
}
