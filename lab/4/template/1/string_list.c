
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


#include "string_list.h"


StringListEntry *CreateAndInitNewStringListEntry (char *str) {

StringListEntry *entry = (StringListEntry*) malloc (sizeof(StringListEntry));

    if (entry == NULL) {
        return NULL;
        }

    entry->value = (char*) malloc (strlen (str) + 1);
    if (!entry->value) {
        free (entry);
        return NULL;
        }

    strcpy (entry->value, str);

    return entry;
}



void FreeStringListEntry (StringListEntry *entry) {

    free (entry->value);
    free (entry);

    return;
}



void AddStringTailStringList (StringListEntry *stringList, char *str) {

    //entry = CreateAndInitNewListEntry (str);
   // AddEntryTailList (stringList, entry);


    return;
}



void FreeStringList (StringListEntry *stringList) {


}


void GenerateRandomStringList (unsigned int size) {


}





