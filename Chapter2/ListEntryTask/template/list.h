

#ifndef _LIST_H_
#define _LIST_H_


typedef struct _ListEntry {
    struct _ListEntry *next;
    struct _ListEntry *prev;
} ListEntry;


ListEntry* CreateAndInitList();

// ������������� ������� ������
void InitList(ListEntry* head);

// ���������� ������ �������� � ����� ������
void InsertTailList(ListEntry* head, ListEntry* newEntry);
void InsertHead(ListEntry* head, ListEntry* newEntry);

void RemoveEntry(ListEntry* entry);
void FreeList(ListEntry* head);

ListEntry* GetNextEntry(ListEntry* entry);
ListEntry* GetPrevEntry(ListEntry* entry);

#endif  // _LIST_H_
