#pragma once

#include "..\Object.h"

typedef void* (*OnEventListner)(Object* sender, struct tagEvent* event, int a, int b);

typedef struct tagEvent {

	EXTENDS_OBJECT;

	__int64 uuid;
	Object* sender;
	OnEventListner handle;

} Event;

#define EXTENDS_EVENT Event __meta

