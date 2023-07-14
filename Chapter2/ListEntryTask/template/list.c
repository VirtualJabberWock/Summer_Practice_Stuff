

#include "list.h"

#include <stdlib.h>


ListEntry* CreateAndInitList()
{
	ListEntry* entry = calloc(1, sizeof(ListEntry));
	InitList(entry);
	return entry;
}

// инициализация пустого списка
void InitList (ListEntry *head) {
	head->next = 0;
	head->prev = 0;
}


// добавление нового элемента в конец списка
void InsertTailList (ListEntry *head, ListEntry *newEntry) {
	ListEntry* node = head;
	while (node->next != 0) {
		node = node->next;
	}
	newEntry->prev = node;
	newEntry->next = 0;
	node->next = newEntry;
	//head->prev = newEntry;
}


// добавление нового элемента в начало списка
void InsertHead (ListEntry *head, ListEntry *newEntry) {
	
	ListEntry* node = head;
	while (node->prev != head) {
		node = node->prev;
	}

	node->prev = newEntry;
	newEntry->next = node;
	newEntry->prev = head->prev;
	head->next = newEntry;
}



void RemoveEntry (ListEntry *entry) {

	if (entry->next != 0) {
		entry->next->prev = entry->prev;
	}
	if (entry->prev != 0) {
		entry->prev->next = entry->next;
	}

	free(entry);
}


void FreeList (ListEntry *head) {

	ListEntry* node = head;
	while (node != 0) {
		ListEntry* next = node->next;
		free(node);
		node = next;
	}

}



ListEntry* GetNextEntry (ListEntry *entry) {
	return entry->next;
}


ListEntry* GetPrevEntry (ListEntry *entry) {
	return entry->prev;
}