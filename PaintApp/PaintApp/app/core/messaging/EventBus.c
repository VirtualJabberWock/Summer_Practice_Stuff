
#include "EventBus.h"
#include "../../container/Map.h"
#include "../../container/map/HashMap.h"

DEFINE_OBJ_TYPE(EventBus);

typedef struct tagEventBus {

	EXTENDS_OBJECT;

	HashMap* map; /* HashMap<EventUUID, linsteners> */

} EventBus;

static EventBus* eventBus_Instance = 0;

EventBus* EventBus_GetInstance();

void EventBus_Dispose(Object* ref)
{
	if (ref != eventBus_Instance) {
		return debugFatalError("Invalid pointer for Singleton Object!");
	}
	if (eventBus_Instance == 0) {
		return;
	}
	DisposeObject(eventBus_Instance->map);
	free(eventBus_Instance);
	eventBus_Instance = 0;
}

EventBus* NewEventBus() 
{
	EventBus* eventBus = calloc(1, sizeof(EventBus));
	if (eventBus == 0) {
		return debugMemError();
	}
	OBJECT_SUPER(EventBus, eventBus);
	OverrideObjectDispose(EventBus, EventBus_Dispose);
	eventBus->map = NewHashMap();
	return eventBus;
}

EventBus* EventBus_GetInstance()
{
	if (eventBus_Instance == 0) {
		eventBus_Instance = NewEventBus();
	}
	return eventBus_Instance;
}

#include "../CommonTypes.h"

DEFINE_OBJ_TYPE(EventListener);

typedef struct tagEventListener {

	EXTENDS_OBJECT;
	Object* optSubscriber;
	OnEventFunc handler;

} EventListener;

void EventBus_postEvent(Event* event)
{
	EventBus* bus = EventBus_GetInstance();
	ObjectVector* subscribers = mapGetByNumber(bus->map, event->__obj.type);

	if (subscribers == 0) {
		return;
	}

	for (int i = 0; i < subscribers->size; i++) {
		EventListener* sub = subscribers->data[i];
		if (sub == 0) continue;
		sub->handler(sub->optSubscriber, event);
	}
}

EventListener* NewEventListener(Object* optSubscriber, OnEventFunc lisntener)
{
	EventListener* eventL = calloc(1, sizeof(EventListener));
	if (eventL == 0) {
		return debugMemError();
	}
	OBJECT_SUPER(EventListener, eventL);
	eventL->optSubscriber = optSubscriber;
	eventL->handler = lisntener;
	return eventL;
}

#if DEBUG_MODE
#include <stdio.h>
#endif

void EventBus_subscribeForEvent(EventUUID uuid, Object* optSubscriber, OnEventFunc handler)
{
	if (uuid == 0) {
		debugFatalErrorFormat("[EventBus] Invalid EventUUID!");
	}

	if (optSubscriber != 0) {
		if (optSubscriber->__addr != optSubscriber) {
			debugFatalErrorFormat("[EventBus] Invalid Subscriber!");
		}
	}

	EventBus* bus = EventBus_GetInstance();

	Long* uuidObject = NewLong(uuid);

	ObjectVector* subscribers = mapGetByNumber(bus->map, uuid);

	if (subscribers == 0) {
		subscribers = NewObjectVector(5, 0);
		mapEmplace(bus->map, uuidObject, subscribers);
	}

	EventListener* listenerObject = NewEventListener(optSubscriber, handler);

	ObjectV_PushBack(subscribers, listenerObject);

#if DEBUG_MODE
	printf("[EventBus] ");
	if (optSubscriber != 0) {
		printf("Object[%s] with ", getObjectTypeName(optSubscriber));
	}
	objectType eventType = (objectType) uuid;
	printf("Handler[%x] subscribed to Event[type = %s]\n", handler, eventType());
#endif

}

