// Шаблон для задания 4.1.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "string_list.h"
#include "sort_string_list.h"


int main () {

    StringListEntry* head = CreateAndInitNewStringListEntry("9");

    AddStringTailStringList(head, "8");
    AddStringTailStringList(head, "7");
    AddStringTailStringList(head, "6");
    AddStringTailStringList(head, "5");
    AddStringTailStringList(head, "4");
    AddStringTailStringList(head, "3");
    AddStringTailStringList(head, "2");
    AddStringTailStringList(head, "1");
    AddStringTailStringList(head, "0");

    IterateList(head, PrintListEntry);

    printf("\n\n"); 
    //SwapListEntry(head->listEntry.prev, &head->listEntry);
    //SwapListEntry(&_1->listEntry, &_2->listEntry);
    //SwapListEntry(&_2->listEntry, &_1->listEntry);
    //SwapListEntry(&_3->listEntry, &_3->listEntry);
    //SwapListEntry(&_3->listEntry, &_3->listEntry);
    //SwapListEntry(&_3->listEntry, &_3->listEntry);

    ListEntry* head_ = SortStringList(head);

    IterateList(GET_CONTENT_RECORD(StringListEntry, listEntry, head_), PrintListEntry);
    //IterateList(head_, PrintListEntry);

    return 0;

}
