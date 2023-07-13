
#ifndef _SORT_STRING_LIST_
#define _SORT_STRING_LIST_

#include "string_list.h"


#define OFFSET_OF(TypeName, entryName)  \
    (unsigned char*)&((TypeName*)0)->entryName

#define GET_CONTENT_RECORD(TypeName, entryName, addr)    \
    ((TypeName*)((unsigned char*)addr - OFFSET_OF(TypeName, entryName)))

void SortStringList (StringListEntry *stringList);

#endif  //_SORT_STRING_LIST_

