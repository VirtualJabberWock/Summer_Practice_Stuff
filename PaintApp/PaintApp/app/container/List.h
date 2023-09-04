#pragma once

#include "..\core\Object.h"
#include <stdlib.h>

typedef struct tagObjectNode {

	Object* obj;
	Object* metadata;
	struct tagObjectNode* next;
	struct tagObjectNode* prev;

} ObjectNode;

typedef struct tagNode {

	void* data;
	struct tagObjectNode* next;
	struct tagObjectNode* prev;

} Node;

typedef struct tagListVTABLE {

	ObjectNode* (*push)(Object* list, Object* obj);
	ObjectNode* (*pushFront)(Object* list, Object* obj);
	/*@return NULLABLE: Return value can be NULL*/
	ObjectNode* (*find)(Object* list, Object* query);
	void (*_clear)(Object* list);
	Object* (*get)(Object* list, int id);
	void (*remove)(Object* list, int id);

};

typedef struct tagList {

	EXTENDS_OBJECT;

	struct tagListVTABLE* __methods;
	ObjectNode* head;
	ObjectNode* tail;
	int size;

} List;

List* NewList(); 

void ListFree(List* list);