// Шаблон для задания 4.1.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "string_list.h"
#include "sort_string_list.h"


int main () {

    StringListEntry tmpStringListEntry;
    ListEntry *entry = &tmpStringListEntry.listEntry;
    printf ("entry: %p\n", entry);
    printf ("start string list: %p\n", GET_CONTENT_RECORD(StringListEntry, listEntry, entry));
    return 0;

}
