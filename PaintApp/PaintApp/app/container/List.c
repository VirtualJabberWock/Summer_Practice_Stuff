#pragma once

#include "List.h"
#include "..\core\DebugUtil.h"
#include "..\core\Object.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

DEFINE_OBJ_TYPE(List);

ObjectNode* List_push(List* self, Object* obj) {

	if (self == 0) {
		return 0;
	}

	ObjectNode* newNode = (ObjectNode*)malloc(sizeof(ObjectNode));
	if (newNode)
	{
		newNode->obj = obj;
		newNode->metadata = 0;
		newNode->next = 0;
		newNode->prev = 0;

		if (self->head == 0)
			self->head = newNode;
		else if (self->tail == 0)
		{
			self->tail = newNode;
			self->tail->prev = self->head;
			self->head->next = self->tail;
		}
		else
		{
			self->tail->next = newNode;
			newNode->prev = self->tail;
			self->tail = newNode;
		}
		self->size++;
	}
	return newNode;
}

ObjectNode* List_pushFront(List* self, Object* obj) {
	if (self == 0) {
		return 0;
	}
	ObjectNode* newNode = (ObjectNode*)malloc(sizeof(ObjectNode));
	if (newNode)
	{
		newNode->obj = obj;
		newNode->metadata = 0;
		newNode->next = 0;
		newNode->prev = 0;

		if (self->head == NULL)
			self->head = newNode;
		else
		{
			self->head->prev = newNode;
			newNode->next = self->head;
			self->head = newNode;
		}
		self->size++;
	}
	return newNode;
}
/*@return NULLABLE: Return value can be NULL*/
ObjectNode* List_find(List* self, Object* query) {
	if (self == 0) {
		return 0;
	}
	ObjectNode* current = 0;
	if (self->head != NULL)
		current = self->head;
	while (current != NULL)
	{
		if (compareObjects(current->obj, query) == 0)
			break;
		current = current->next;
	}
	return current;
}

void _List_clear(List* self) {

	if (self == 0) {
		return 0;
	}

	ObjectNode* current = self->head;
	ObjectNode* tmp;
	while (current != NULL)
	{
		tmp = current->next;
		DisposeObject(current->obj);
		DisposeObject(current->metadata);
		if(current->obj != 0) free(current->obj);
		if (current->metadata != 0) free(current->metadata);
		free(current);
		current = tmp;
	}
	self->head = 0;
	self->tail = 0;
	self->size = 0;
}

Object* List_get(List* self, int id) {

	if (self == 0) {
		return 0;
	}

	if ((id >= self->size) || (id < 0)) {
		return NULL;
	}
	if (id == 0) {
		return self->head->obj;
	}
	if (id == self->size - 1) {
		return self->tail->obj;
	}

	if (self->size / 2 - id > 0) {
		ObjectNode* current = self->head;
		for (int i = 0; i < id; i++) {
			if (current == 0)
				return NULL;
			current = current->next;
		}
		return current->obj;
	}
	else {
		ObjectNode* current = self->tail;
		for (int i = self->size-1; i > id; i--) {
			if (current == 0)
				return NULL;
			current = current->prev;
		}
		return current->obj;
	}
}

void List_remove(List* self, int id) {
	if (self == 0) return 0;
	if (id == 0) {
		if (self->head == 0) return NULL;
		ObjectNode* tmp2 = self->head->next;
		free(self->head);
		self->head = tmp2;
		self->size--;
		return 1;
	}
	if (id == self->size - 1) {
		if (self->tail == 0) return NULL;
		ObjectNode* tmp2 = self->tail->prev;
		if (tmp2 != 0) tmp2->next = 0;
		free(self->tail);
		self->tail = tmp2;
		self->size--;
		return 1;
	}
	ObjectNode* current = self->head;
	ObjectNode* previous = current;
	for (int i = 0; i < id; i++) {
		previous = current;
		current = current->next;
		if (current == 0) return 0;
	}
	previous->next = current->next;
	DisposeObject(current->obj);
	DisposeObject(current->metadata);
	if (current->obj != 0) free(current->obj);
	if (current->metadata != 0) free(current->metadata);
	free(current);
	if (id == self->size - 1) {
		self->tail = previous;
	}
	self->size--;
	return 1;
}

struct tagListVTABLE List_METHODS[] = { { 
		List_push, List_pushFront, List_find, _List_clear, List_get, List_remove 
} };

List* NewList()
{
	List* list = (List*)malloc(sizeof(List));
	if (list == 0) {
		return 0;
	}
	OBJECT_SUPER(List, list);
	OverrideObjectDispose(List, ListFree);
	list->tail = 0;
	list->head = 0;
	list->size = 0;
	list->__methods = List_METHODS;
	return list;
}

void ListFree(List* list)
{
	list->__methods->_clear(list);
}

