#include "sort_list.h"
#include "string_list.h"

#include <stdio.h>

//int areTheyNeighbours(ListEntry* A, ListEntry* B) {
//    return (A->next == B && B->prev == A) || (A->prev == B && B->next == A);
//}

void SwapListEntry(ListEntry* node1, ListEntry* node2) {

	if (node1 == node2) return;

	/*ListEntry* next1 = node1->next;
	ListEntry* next2 = node2->next;
	ListEntry* prev1 = node1->prev;
	ListEntry* prev2 = node2->prev;

	if (node1->next == node2) {
		node1->next = next2;
		node1->prev = node2;
		node2->next = node1;
		node2->prev = prev1;
		prev1->next = node2;
		next2->prev = node1;
	}
	else if (node2->next == node1) {
		node2->next = next1;
		node2->prev = node1;
		node1->next = node2;
		node1->prev = prev2;
		prev2->next = node1;
		next1->prev = node2;
	}
	else {
		node1->next = next2;
		node2->next = next1;
		node1->prev = prev2;
		node2->prev = prev1;
		prev1->next = node2;
		prev2->next = node1;
		next1->prev = node2;
		next2->prev = node1;
	}*/

	char* tmp = GET_STRING(node1);
	GET_STRING(node1) = GET_STRING(node2);
	GET_STRING(node2) = tmp;

}

#define _HEAD head

void SwapListEntryAndPointers(ListEntry** a, ListEntry** b) {
    //SwapListEntry(*a, *b);
}

ListEntry* partitionList(ListEntry* head, ListEntry* end, EntryCompareFunction cmpFunc) {

	ListEntry* pivot = end;
	ListEntry* i = (_HEAD)->prev;
	ListEntry* j = (_HEAD);
	ListEntry* headBefore = (_HEAD);
	ListEntry* iNext = (i == NULL) ? (_HEAD) : i->next;
	int iter = 0;
	while (j != end) {
		ListEntry* jNext = j->next;
		int check = cmpFunc(pivot, j);
		if (check >= 0) {
			i = iNext;
			iNext = (i == NULL) ? (_HEAD) : i->next;
			SwapListEntry(i, j);
		}
		j = jNext;
		iter++;
	}


	i = (i == NULL) ? (_HEAD) : i->next;
	SwapListEntry(i, end);
	//*head = *end;
	//IterateList(GET_CONTENT_RECORD(StringListEntry, listEntry, (_HEAD)), PrintListEntry);
	return i;
}

static void quickSortAlg(ListEntry* head, ListEntry* end, EntryCompareFunction cmpFunc) {
	if (end != NULL && head != end && head != end->next) {
		ListEntry* pivot = partitionList(head, end, cmpFunc);
		quickSortAlg(head, pivot->prev, cmpFunc);
		quickSortAlg(pivot->next, end, cmpFunc);
	}
}

ListEntry* QuickSortList(ListEntry* head, EntryCompareFunction cmpFunc)
{

	/*ListEntry* node = head;
	while (node->next != 0) {
		node = node->next;
	}*/
	//quickSortAlg(head, head->prev, cmpFunc);

	ListEntry* end = head->next;

	int len = 1;
	while (end != head) {
		len++;
		end = end->next;
	}
	
	ListEntry* iNode = head;
	ListEntry* jNode = head;
	for (int i = 0; i < len; i++) {
		ListEntry* iNodeNext = iNode->next;
		for (int j = 0; j < len; j++) {
	
			ListEntry* jNodeNext = jNode->next;
			if (cmpFunc(jNode->next, jNode) < 0) {
				SwapListEntry(jNode->next, jNode);
			}
			jNode = jNodeNext;
		}
		iNode = iNodeNext;
	}
	return iNode;
}
