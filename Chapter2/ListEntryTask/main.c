// Шаблон для задания 4.1.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "string_list.h"
#include "sort_string_list.h"


int main () {

    StringListEntry* head = CreateAndInitNewStringListEntry("4");

    AddStringTailStringList(head, "3");
    AddStringTailStringList(head, "2");
    AddStringTailStringList(head, "1");

    IterateList(head, PrintListEntry);

    printf("\n\n");
    ListEntry* head_ = &head->listEntry;
    //SwapListEntry(&head_, &head->listEntry, &b->listEntry);

    SortStringList(head);

    IterateList(GET_CONTENT_RECORD(StringListEntry, listEntry, head_), PrintListEntry);

    return 0;

}
