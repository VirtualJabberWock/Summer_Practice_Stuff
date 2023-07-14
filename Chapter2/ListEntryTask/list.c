

#include "list.h"

#include <stdlib.h>


ListEntry* CreateAndInitList()
{
	ListEntry* entry = calloc(1, sizeof(ListEntry));
	InitList(entry);
	return entry;
}

// ������������� ������� ������
void InitList (ListEntry *head) {
	head->next = head;
	head->prev = head;
}


// ���������� ������ �������� � ����� ������
void InsertTailList (ListEntry *head, ListEntry *newEntry) {
	ListEntry* node = head;
	while (node->next != head) {
		node = node->next;
	}
	newEntry->prev = node;
	newEntry->next = head;
	node->next = newEntry;
	head->prev = newEntry;
}


// ���������� ������ �������� � ������ ������
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

	ListEntry* node = head->next;
	while (node != head) {
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