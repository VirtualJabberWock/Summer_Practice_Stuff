#pragma once
#include "HashMap.h"
#include "..\List.h"
#include "..\Map.h"

DEFINE_MAP_TYPE(HashMap);

void HashMap_emplace(struct tagHashMap* self, Object* key, Object* value) {

	if (self == 0 || key == 0) {
		return 0;
	}

	__int64 h = getObjectHash(key);
	__int64 index = abs(h % self->static_size);


	if (self->data->data[index] == NULL) {
		self->data->data[index] = NewList();
	}

	List* l = (List*)(self->data->data[index]);
	ObjectNode* node = l->__methods->push(l, value);
	node->metadata = key;
}

Object* HashMap_get(struct tagHashMap* self, Object* key) {

	if (key == NULL) {
		return 0;
	}

	__int64 h = getObjectHash(key);

	__int64 index = abs(h % self->static_size);
	if (index > self->static_size) {
		return 0;
	}

	if (self->data->data[index] == NULL) {
		return 0;
	}

	List* l = self->data->data[index];

	ObjectNode* current = l->head;
	if (l->head == NULL) {
		return 0;
	}
	

	while (current != NULL)
	{
		if (compareObjects(key, current->metadata) == 0) {
			return current->obj;
		}
		current = current->next;
	}

	return 0;

}

#define DEFAULT_SIZE 100000

HashMap* NewHashMap()
{
	HashMap* map = (HashMap*)malloc(sizeof(HashMap));
	if (map == 0) {
		return 0;
	}
	
	MAP_SUPER(HashMap, map);

	OverrideObjectDispose(HashMap, FreeHashMap);

	OverrideIMapEmplace(HashMap, HashMap_emplace);
	OverrideIMapGet(HashMap, HashMap_get);

	map->data = NewObjectVector(DEFAULT_SIZE, DEFAULT_SIZE);
	map->static_size = DEFAULT_SIZE;

	for (int i = 0; i < map->static_size; i++) {
		map->data->data[0] = 0;
	}

	return map;

}

void FreeHashMap(HashMap* hashmap)
{
	hashmap->static_size = DEFAULT_SIZE;
	DisposeObject(hashmap->data);
	free(hashmap->data);
	hashmap->data = 0;
}
