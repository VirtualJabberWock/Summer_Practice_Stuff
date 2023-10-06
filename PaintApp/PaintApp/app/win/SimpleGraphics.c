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
	LineTo(hdc, l, t);
}

void DrawRectInternal(HDC hdc, RECT* rect, HBRUSH brush, int width)
{
	RECT leftSide = {
		rect->left, rect->top,
		rect->left+width, rect->bottom
	};
	RECT topSide = {
		rect->left, rect->top,
		rect->right, rect->top + width
	};
	RECT rightSide = {
		rect->right - width, rect->top,
		rect->right, rect->bottom
	};
	RECT bottomSide = {
		rect->left, rect->bottom-width,
		rect->right, rect->bottom
	};
	FillRect(hdc, &leftSide, brush);
	FillRect(hdc, &topSide, brush);
	FillRect(hdc, &rightSide, brush);
	FillRect(hdc, &bottomSide, brush);
}

#include <intrin.h>

void DrawBitmap(HDC hdc, HBITMAP hBitmap, int x, int y)
{
	if (hBitmap == 0) {
		printf("[WARNING] [SimpleGraphics] Attempt draw a NULL bitmap {func(caller) = %x}", _ReturnAddress());
		return;
	}

	HDC hdcMem = CreateCompatibleDC(hdc);
	BITMAP bitmap;
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

	GetObject(hBitmap, sizeof(bitmap), &bitmap);
	BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
}

