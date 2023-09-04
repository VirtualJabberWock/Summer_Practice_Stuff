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
	ObjectNode* node;
	if (l->tail != 0) {
		if (compareObjects(l->tail->metadata, key) == 0) {
			l->tail->obj = value;
			return;
		}
	}
	node = l->__methods->push(l, value);
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

void HashMap_iterateKeys(HashMap* map, ObjectIterator iterator)
{
	int j = 0;
	for (int i = 0; i < map->data->size; i++) {
	
		List* list = map->data->data[i];
		if (list == 0) continue;

		ObjectNode* current = list->head;

		if (list->head == NULL) {
			return;
		}

		while (current != NULL)
		{
			iterator(map, current->metadata, j);
			current = current->next;
			j++;
		}
	}
}

#define DEFAULT_SIZE 200

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
	OverrideIMapIterateKeys(HashMap, HashMap_iterateKeys);

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
