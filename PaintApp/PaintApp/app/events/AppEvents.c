#include "AppEvents.h"
#pragma once

DEFINE_OBJ_TYPE(UserCommandEvent);

static void FreeUserCommandEvent(UserCommandEvent* event)
{
	if (event->cmd != 0) {
		DestroyObject(&event->cmd);
		event->cmd = 0;
	}
}

UserCommandEvent* NewUserCommandEvent(char* buffer)
{
	UserCommandEvent* e = CreateCommonEvent(UserCommandEvent);
	OBJECT_SUPER(UserCommandEvent, e);
	e->cmd = NewString(buffer);
	e->cmd->data[e->cmd->length - 1] = 0;
	e->cmd->length--;
	e->status = 0;
	OverrideObjectDispose(UserCommandEvent, FreeUserCommandEvent);
	return e;
}
