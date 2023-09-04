#include "SimpleGraphics.h"



void DrawLine(HDC hdc, int x, int y, int x2, int y2, HPEN pen)
{
	(HPEN)SelectObject(hdc, pen);
	MoveToEx(hdc, x, y, NULL);
	LineTo(hdc, x2, y2);
}

void DrawRect(HDC hdc, RECT* rect, HPEN pen)
{
	(HPEN)SelectObject(hdc, pen);
	int l = rect->left;
	int t = rect->top;
	int r = rect->right;
	int b = rect->bottom;
	MoveToEx(hdc, l, t, NULL);
	LineTo(hdc, r, t);
	MoveToEx(hdc, r, t, NULL);
	LineTo(hdc, r, b);
	MoveToEx(hdc, r, b, NULL);
	LineTo(hdc, l, b);
	MoveToEx(hdc, l, b, NULL);
	LineTo(hdc, l, t-1);
}

