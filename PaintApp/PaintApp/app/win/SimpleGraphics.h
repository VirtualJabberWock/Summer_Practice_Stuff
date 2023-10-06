#pragma once

#include <Windows.h>

#define GET_EXPAND_RECT(rect, name, l, t, r, b) \
RECT __##name = { \
rect.left - (l),\
rect.top - (t),\
rect.right + (r),\
rect.bottom + (b)\
}; \


#define GET_NORMALIZED_RECT(rect, name) \
RECT __##name = { \
min(rect.left, rect.right),\
min(rect.top, rect.bottom),\
max(rect.left, rect.right),\
max(rect.top, rect.bottom)\
}; \

void DrawLine(HDC hdc, int x, int y, int x2, int y2, HPEN pen);
void DrawRect(HDC hdc, RECT* rect, HPEN pen);

void DrawRectInternal(HDC hdc, RECT* rect, HBRUSH brush, int width);

void DrawBitmap(HDC hdc, HBITMAP bitmap, int x, int y);