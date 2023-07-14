
#ifndef _STRING_LIST_H_
#define _STRING_LIST_H_


#define OFFSET_OF(TypeName, entryName)  \
    (unsigned char*)&((TypeName*)0)->entryName

#define GET_CONTENT_RECORD(TypeName, entryName, addr)    \
    ((TypeName*)((unsigned char*)addr - OFFSET_OF(TypeName, entryName)))

#define GET_STRING(addr)    \
    ((StringListEntry*)((unsigned char*)addr - OFFSET_OF(StringListEntry, listEntry)))->value




#include "list.h"

typedef struct _StringListEntry {

    char *value;
    ListEntry listEntry;

} StringListEntry;

#endif  // _STRING_LIST_H_

StringListEntry* CreateAndInitNewStringListEntry(char* str);

void FreeStringListEntry(StringListEntry* entry);
StringListEntry* AddStringTailStringList(StringListEntry* stringList, char* str);
void FreeStringList(StringListEntry* stringList);
void GenerateRandomStringList(unsigned int size);

typedef void (*StringListIteration)(StringListEntry* entry, int index);

void IterateList(StringListEntry* head, StringListIteration onIteration);

void PrintListEntry(StringListEntry* head, int index);
