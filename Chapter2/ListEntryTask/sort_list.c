#include "sort_list.h"
#include "string_list.h"

#include <stdio.h>

//int areTheyNeighbours(ListEntry* A, ListEntry* B) {
//    return (A->next == B && B->prev == A) || (A->prev == B && B->next == A);
//}

void SwapListEntry(ListEntry* node1, ListEntry* node2) {


	if (node1->next == node2 || node2->next == node1) {
		ListEntry* next1 = node1->next;
		ListEntry* next2 = node2->next;
		node1->next = next2;
		node2->next = next1;
		if (next1 != NULL)
			next1->prev = node2;
		if (next2 != NULL)
			next2->prev = node1;
	}
	else {
		ListEntry* prev1 = node1->prev;
		ListEntry* prev2 = node2->prev;
		node1->prev = prev2;
		node2->prev = prev1;
		ListEntry* next1 = node1->next;
		ListEntry* next2 = node2->next;
		node1->next = next2;
		node2->next = next1;
		if (next1 != NULL)
			next1->prev = node2;
		if (next2 != NULL)
			next2->prev = node1;
	
		if (prev1 != NULL)
			prev1->next = node2;
		if (prev2 != NULL)
			prev2->next = node1;
	}


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
	IterateList(GET_CONTENT_RECORD(StringListEntry, listEntry, (_HEAD)), PrintListEntry);
	return i;
}

static void quickSortAlg(ListEntry* head, ListEntry* end, EntryCompareFunction cmpFunc) {
	if (end != NULL && head != end && head != end->next) {
		ListEntry* pivot = partitionList(head, end, cmpFunc);
		quickSortAlg(head, pivot->prev, cmpFunc);
		quickSortAlg(pivot->next, end, cmpFunc);
	}
}

ListEntry* QuickSortEntryList(ListEntry* head, EntryCompareFunction cmpFunc)
{

	ListEntry* node = head;
	while (node->next != 0) {
		node = node->next;
	}
	quickSortAlg(head, node, cmpFunc);

}
