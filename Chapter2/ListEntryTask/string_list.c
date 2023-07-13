
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

    strcpy_s(entry->value, len, str);

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





