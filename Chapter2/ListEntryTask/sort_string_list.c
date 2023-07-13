

#include "sort_string_list.h"



void SortStringList (StringListEntry *stringList) {
/* Замечание: 
Поменять местами два элемента в сортируемом контейнере
для списка должно означать "поменять местами два элемента списка".
*/

    StringListEntry *head = stringList;
    StringListEntry *currentStringListEntry = stringList;
    ListEntry *currentListEntry;

    // пример обхода всех элементов списка
    currentStringListEntry = head;
    currentListEntry = &currentStringListEntry->listEntry;
    do {
        currentStringListEntry = GET_CONTENT_RECORD(StringListEntry, listEntry, currentListEntry);

        //...

        //currentEntry = currentEntry->next;

        currentListEntry = GetNextEntry (currentListEntry);
        
    } while (currentStringListEntry != head);

}
