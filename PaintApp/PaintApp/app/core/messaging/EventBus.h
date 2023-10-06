#pragma once
#include "../Object.h"
#include "Event.h"

void EventBus_postEvent(Event* event);
void EventBus_subscribeForEvent(EventUUID uuid, Object* optSubscriber, OnEventFunc handlerFunc);