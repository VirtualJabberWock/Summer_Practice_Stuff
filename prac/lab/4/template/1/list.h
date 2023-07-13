

#ifndef _LIST_H_
#define _LIST_H_


typedef struct _ListEntry {
    struct _ListEntry *next;
    struct _ListEntry *prev;
} ListEntry;

ListEntry *GetNextEntry (ListEntry *entry);

#endif  // _LIST_H_
