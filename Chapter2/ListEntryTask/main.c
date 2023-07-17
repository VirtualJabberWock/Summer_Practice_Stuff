// Шаблон для задания 4.1.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "string_list.h"
#include "sort_string_list.h"


int main () {

    StringListEntry* head =
        CreateAndInitNewStringListEntry("f");
    AddStringTailStringList(head, "a");
    AddStringTailStringList(head, "c");
    AddStringTailStringList(head, "b");
    AddStringTailStringList(head, "g");
    AddStringTailStringList(head, "d");
    AddStringTailStringList(head, "e");
    AddStringTailStringList(head, "i");
    AddStringTailStringList(head, "h");

    IterateList(head, PrintListEntry);

    printf("\n");

    ListEntry* head_ = SortStringList(head);

    printf("\n");

    IterateList(GET_CONTENT_RECORD(StringListEntry, listEntry, head_), PrintListEntry);
    //IterateList(head_, PrintListEntry);

    return 0;

}
