

#include "sort_string_list.h"

#include <string.h>

static int CompareTwoStringEntries(ListEntry* a, ListEntry* b) {

    if (a == 0 || b == 0) return 0;
    return strcmp(
        GET_CONTENT_RECORD(StringListEntry, listEntry, a)->value,
        GET_CONTENT_RECORD(StringListEntry, listEntry, b)->value
    );
}


#include "sort_list.h"

ListEntry* SortStringList (StringListEntry *stringList) {
/* ���������: 
�������� ������� ��� �������� � ����������� ����������
��� ������ ������ �������� "�������� ������� ��� �������� ������".
*/


    ListEntry* entry = &stringList->listEntry;

    return QuickSortList(entry, CompareTwoStringEntries);

}
