#pragma once

#include "Object.h"
#include <stdlib.h>
#include "CommonTypes.h"

typedef struct tagObjectNode {

	Object* obj;
	struct tagObjectNode* next;
	struct tagObjectNode* prev;
	__int64 hash;

} ObjectNode;

typedef struct tagNumberNode {

	__int64 number;
	struct tagObjectNode* next;
	struct tagObjectNode* prev;

} NumberNode;

typedef struct tagNode {

	void* data;
	struct tagObjectNode* next;
	struct tagObjectNode* prev;

} Node;

OBJECT_CLASS_FM(ObjectList,

	ObjectNode* head;
	ObjectNode* tail;
	int size;
,

	ObjectNode* (*push)(struct tagObjectList* self, Object* obj);
	ObjectNode* (*pushFront)(struct tagObjectList* self, Object* obj);
	/*@return NULLABLE: Return value can be NULL*/
	ObjectNode* (*find)(struct tagObjectList* self, Object* query);
	void (*_clear)(struct tagObjectList* self);
	Object* (*get)(struct tagObjectList* self, int id);
	void (*remove)(struct tagObjectList* self, int id);

)

ObjectList* NewObjectList(); 

void ListFree(ObjectList* list);
String* ListToString(ObjectList* list);