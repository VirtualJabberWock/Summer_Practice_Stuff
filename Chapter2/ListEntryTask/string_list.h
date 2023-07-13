
#ifndef _STRING_LIST_H_
#define _STRING_LIST_H_







#include "list.h"

typedef struct _StringListEntry {

    char *value;
    ListEntry listEntry;

} StringListEntry;

#endif  // _STRING_LIST_H_
