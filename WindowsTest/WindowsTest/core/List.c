#pragma once

#include "List.h"
#include "DebugUtil.h"
#include "Object.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

DEFINE_TYPE(List);

ObjectNode* List_push(List* self, Object* obj) {

	if (self == 0) throw NULL_POINTER_EXCEPTION;

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
	if (self == 0) throw NULL_POINTER_EXCEPTION;
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
	if (self == 0) throw NULL_POINTER_EXCEPTION;
	ObjectNode* current = 0;
	if (self->head != NULL)
		current = self->head;
	while (current != NULL)
	{
		if (isObjectEquals(current->obj, query))
			break;
		current = current->next;
	}
	return current;
}

void _List_clear(List* self) {
	if (self == 0) throw NULL_POINTER_EXCEPTION;

	ObjectNode* current = self->head;
	ObjectNode* tmp;
	while (current != NULL)
	{
		tmp = current->next;
		if (isObject(current->obj)) {
			if(current->obj->free != 0 && 
				current->obj->free != standartFree)
			current->obj->free(current->obj);
		}
		if (isObject(current->metadata)) {
			if(current->metadata->free != 0 && 
				current->metadata->free != standartFree)
			current->metadata->free(current->obj);
		}
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

	if (self == 0) throw NULL_POINTER_EXCEPTION;

	if ((id >= self->size) || (id < 0))
		return NULL;
	if (id == 0) return self->head->obj;
	if (id == self->size - 1) return self->tail->obj;

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
	if (self == 0) throw NULL_POINTER_EXCEPTION;
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
	if (isObject(current->obj)) {
		if (current->obj->free != 0 &&
			current->obj->free != standartFree)
			current->obj->free(current->obj);
	}
	if (isObject(current->metadata)) {
		if (current->metadata->free != 0 &&
			current->metadata->free != standartFree)
			current->metadata->free(current->obj);
	}
	if (current->obj != 0) free(current->obj);
	if (current->metadata != 0) free(current->metadata);
	free(current);
	if (id == self->size - 1) {
		self->tail = previous;
	}
	self->size--;
	return 1;
}

struct List_mtable_tag List_METHODS[] = { { List_push, List_pushFront, List_find, _List_clear, List_get, List_remove } };

List* NewList()
{
	List* list = (List*)malloc(sizeof(List));
	if (list == 0) MEM_PANIC_EXCEPTION;
	OBJECT_SUPER_FM(List, list);
	list->tail = 0;
	list->head = 0;
	list->size = 0;
	list->free = ListFree;
	return list;
}

void ListFree(List* list)
{
	if (!isObject(list))return;
	if (CHECK_OBJ_TYPE(list, List) == 0)return;
	list->_->_clear(list);
}

#include "CommonTypes.h"

String* ListToString(List* list)
{
	String* s = NewString("List : {\n");
	for (ObjectNode* node = list->head; node != NULL; node = node->next) {
		char buf[3000];
		if (node->obj == 0) {
			sprintf_s(buf, 3000, "    NULL (= 0), \n");
			s->_->add(s, buf);
			continue;
		}
		if (isObject(node->obj) == 0) {
			sprintf_s(buf, 3000, "    ???? (= %p), \n", node->obj);
			s->_->add(s, buf);
			continue;
		}
		ObjectType n_type = node->obj->type;
		if (checkObjectType(node->obj, String_TYPE) == 0) {
			if (n_type != List_TYPE) {
				if (n_type == Float_TYPE) sprintf_s(buf, 3000, "    %s[%f], \n", "Float", ((Float*)node->obj)->value);
				else if(n_type == Double_TYPE) sprintf_s(buf, 3000, "    %s[%llf], \n", "Double", ((Double*)node->obj)->value);
				else sprintf_s(buf, 3000, "    %s[%lld], \n", ((ObjectType)(node->obj->type))(), hashObject(node->obj));
				s->_->add(s, buf);
				continue;
			}
			else {
				List* _list = node->obj;
				sprintf_s(buf, 3000, "    List[%p] (size = %d), \n", node->obj, _list->size);
				s->_->add(s, buf);
				continue;
			}
		}
		if (checkObjectType(node->obj, String_TYPE) == 1) {
			sprintf_s(buf, 3000, "    %s, \n", CastString(node->obj)->ptr);
			s->_->add(s, buf);
			continue;
		}
	}
	s->_->add(s, "}\n");
	return s;
}
