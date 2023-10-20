#pragma once

#include "../core/messaging/Event.h"
#include "../core/String.h"

typedef struct tagUserCommandEvent
{
	EXTENDS_EVENT;

	String* cmd;
	int status;

} UserCommandEvent;

UserCommandEvent* NewUserCommandEvent(char* buffer);

typedef Event ImageChangedEvent;
Event* NewImageChangedEvent();