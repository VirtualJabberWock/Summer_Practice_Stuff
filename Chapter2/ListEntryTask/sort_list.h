#pragma once

#include "list.h"


void SwapListEntry(ListEntry* a, ListEntry* b);
void SwapListEntryAndPointers(ListEntry** a, ListEntry** b);

typedef int (*EntryCompareFunction) (ListEntry* entry, ListEntry* with);

ListEntry* QuickSortList(ListEntry* head, EntryCompareFunction cmpFunc);