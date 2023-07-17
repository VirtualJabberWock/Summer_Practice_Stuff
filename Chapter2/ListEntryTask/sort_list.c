#include "sort_list.h"
#include "string_list.h"

#include <stdio.h>

//int areTheyNeighbours(ListEntry* A, ListEntry* B) {
//    return (A->next == B && B->prev == A) || (A->prev == B && B->next == A);
//}

static ListEntry* beforeHead = 0;

typedef struct tagChunk {

	ListEntry* head;
	ListEntry* end;
	ListEntry* pivot;

} Chunk;

void SwapListEntry(ListEntry* node1, ListEntry* node2) {

	//printf("Swap [%s] to [%s] \n", GET_STRING(node1), GET_STRING(node2));
	if (node1 == node2) return;

	ListEntry* next1 = node1->next;
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
	}

	/*char* tmp = GET_STRING(node1);
	GET_STRING(node1) = GET_STRING(node2);
	GET_STRING(node2) = tmp;*/

}


void SwapListEntryAndPointers(ListEntry** a, ListEntry** b) {
    //SwapListEntry(*a, *b);
}

ListEntry* partitionList(ListEntry* head, ListEntry* end, EntryCompareFunction cmpFunc, Chunk* chunk) {

	//ListEntry* node = head;
	//printf("Partition: ");
	//do {
	//	printf("%s ", GET_STRING(node));
	//	node = node->next;
	//} while (node != end->next);

	ListEntry* pivot = end;
	ListEntry* fast = head->next;
	ListEntry* slow = head;
	char flag = 0;
	char swapFlag = 0;
	while (fast != pivot)
	{
		ListEntry* fastNext = fast->next;
		flag = 1;
		if (cmpFunc(pivot, fast) > 0) {

			ListEntry* slowNext = slow->next;

			if (fast == head) head = slow;
			if (slow == head) head = fast;
			if (slow == end) end = fast;
			if (fast == end) end = slow;
			SwapListEntry(fast, slow);
			slow = fast->next;
			swapFlag = 1;
		}

		fast = fastNext;
	}


	if (cmpFunc(slow, pivot) <= 0) {
		slow = slow->next;
	}

	if (pivot == head) head = slow;
	if (slow == head) head = pivot;
	if (slow == end) end = pivot;
	if (pivot == end) end = slow;
	SwapListEntry(pivot, slow);
	


	chunk->head = head;
	chunk->end = end;
	chunk->pivot = pivot;

	//node = head;
	//printf("-> ");
	//do {
	//	printf("%s ", GET_STRING(node));
	//	node = node->next;
	//} while (node != end->next);

	//printf("{Pivot = [%s], Head = [%s], End = [%s]}\n", GET_STRING(pivot), GET_STRING(head), GET_STRING(end));

	return pivot;
}

static ListEntry* quickSortAlg(ListEntry* head, ListEntry* end, EntryCompareFunction cmpFunc) {
	Chunk chunk = { 0,0,0,};
	if (head != end) {

		partitionList(head, end, cmpFunc, &chunk);

		if (chunk.pivot != chunk.head) {
			quickSortAlg(chunk.head, chunk.pivot->prev, cmpFunc);
		}
		if (chunk.pivot != chunk.end) {
			quickSortAlg(chunk.pivot->next, chunk.end, cmpFunc);
		}
	}
	return chunk.head;
}

ListEntry* QuickSortList(ListEntry* head, EntryCompareFunction cmpFunc)
{
	beforeHead = head;
	return quickSortAlg(head, head->prev, cmpFunc);
}
