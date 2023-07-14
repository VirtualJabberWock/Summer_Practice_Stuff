
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


#include "string_list.h"


StringListEntry *CreateAndInitNewStringListEntry (char *str) {

StringListEntry *entry = (StringListEntry*) malloc (sizeof(StringListEntry));

    if (entry == NULL) {
        return NULL;
    }

    int len = strlen(str) + 1;

    entry->value = (char*) malloc (len);
    if (!entry->value) {
        free (entry);
        return NULL;
        }

    InitList(&entry->listEntry);

    strcpy_s(entry->value, len, str);

    return entry;
}




void FreeStringListEntry (StringListEntry *entry) {

    free (entry->value);
    free (entry);

    return;
}



StringListEntry* AddStringTailStringList (StringListEntry *stringList, char *str) {

    StringListEntry* entry = CreateAndInitNewStringListEntry(str);
    InsertTailList(&stringList->listEntry, &entry->listEntry);

    return entry;
}



void FreeStringList (StringListEntry *stringList) {

    StringListEntry* currentStringListEntry = stringList;
    ListEntry* currentListEntry;

    currentListEntry = &currentStringListEntry->listEntry;
    currentStringListEntry = GET_CONTENT_RECORD(StringListEntry, listEntry, currentListEntry);
    int index = 0;
    do {

        currentListEntry = GetNextEntry(currentListEntry);

        FreeStringListEntry(currentStringListEntry);

        currentStringListEntry = GET_CONTENT_RECORD(StringListEntry, listEntry, currentListEntry);

        index++;

    } while (currentStringListEntry != stringList);

}


void GenerateRandomStringList (unsigned int size) {



}

void IterateList(StringListEntry* head, StringListIteration onIteration)
{
    StringListEntry* currentStringListEntry = head;
    ListEntry* currentListEntry;

    currentListEntry = &currentStringListEntry->listEntry;
    currentStringListEntry = GET_CONTENT_RECORD(StringListEntry, listEntry, currentListEntry);
    int index = 0;
    do {

        onIteration(currentStringListEntry, index);

        currentListEntry = GetNextEntry(currentListEntry);

        currentStringListEntry = GET_CONTENT_RECORD(StringListEntry, listEntry, currentListEntry);

        index++;

    } while (currentListEntry != 0);
}

#include <stdio.h>

void PrintListEntry(StringListEntry* head, int index)
{
    printf("%d. %s\n", index, head->value);
}





