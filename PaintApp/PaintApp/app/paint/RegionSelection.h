#pragma once

#include <Windows.h>

typedef struct tagSelection {

	RECT rect;
	int state;

} Selection;

#define REG_SELECT_CURSOR_IN 0

void InitializeSelectionResources();

void NotifySelection(Selection* select, int x, int y, HWND hWnd);
void CompleteSelection(Selection* select, int x, int y);
void StartSelection(Selection* select, int x, int y);
void DrawSelection(HDC hdc, Selection* select, int xOffset, int yOffset);
int isPointInSelection(Selection* select, int x, int y);

HCURSOR RegSelect_GetCursor(int id);

void InvalidateSelection(Selection* select, HWND hWnd);