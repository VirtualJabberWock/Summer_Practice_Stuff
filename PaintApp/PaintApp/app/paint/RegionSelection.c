#pragma once
#include "RegionSelection.h"
#include "..\win\SimpleGraphics.h"
#include "..\win_classes\CanvasWindow.h"

static HBRUSH selectBrush;

static HPEN selectionPen;
static HPEN selectionPenComplete;

static HCURSOR cursorMove;

static int isSelectionResourcesInitialized = 0;

void InitializeSelectionResources() {
	if (isSelectionResourcesInitialized == 1) {
		return;
	}
	selectBrush = CreateSolidBrush(0x00ff9999);
	selectionPen = CreatePen(PS_DOT, 1, 0x555555);
	selectionPenComplete = CreatePen(PS_DASHDOT, 1, 0xee8800);
	cursorMove = LoadCursor(0, IDC_ARROW);
	isSelectionResourcesInitialized = 1;
}


void NotifySelection(Selection* select, int x, int y, HWND hWnd)
{


	if (select->state != 1) return;
	select->rect.right = x;
	select->rect.bottom = y;
	InvalidateRect(hWnd, 0, 0);
}

void DrawSelection(HDC hdc, Selection* select, int xOffset, int yOffset)
{
	if (select->state == 0) return;
	RECT rect = {
		select->rect.left - xOffset,
		select->rect.top - yOffset,
		select->rect.right - xOffset,
		select->rect.bottom - yOffset,
	};
	if (select->state == 1) {
		//FillRect(hdc, &selectionRegion, selectBrush);
		DrawRect(hdc, &rect, selectionPen);
	}
	if (select->state == 2) {
		DrawRect(hdc, &rect, selectionPenComplete);
	}
}

void CompleteSelection(Selection* select, int x, int y)
{
	if (select->state == 1 &&
		(select->rect.top != select->rect.bottom) &&
		(select->rect.left != select->rect.right)
		) {
		select->state = 2;
	}
	else {
		select->state = 0;
	}
}

void StartSelection(Selection* select, int x, int y)
{
	if (isSelectionResourcesInitialized == 0) {
		InitializeSelectionResources();
	}
	select->rect.left = x;
	select->rect.right = x;
	select->rect.top = y;
	select->rect.bottom = y;

	select->state = 1;
}

int isPointInSelection(Selection* select, int x, int y) {

	if (select->state != 2) return 0;

	int l = min(select->rect.left, select->rect.right);
	int r = max(select->rect.left, select->rect.right);
	int t = min(select->rect.top, select->rect.bottom);
	int b = max(select->rect.top, select->rect.bottom);

	if (x >= l && x <= r && y > t && y < b) {
		return 1;
	}

	return 0;
}

HCURSOR RegSelect_GetCursor(int id)
{
	if (id == REG_SELECT_CURSOR_IN) {
		return cursorMove;
	}
	return 0;
}

void InvalidateSelection(Selection* select, HWND hWnd)
{


	int l = min(select->rect.left, select->rect.right);
	int r = max(select->rect.left, select->rect.right);
	int t = min(select->rect.top, select->rect.bottom);
	int b = max(select->rect.top, select->rect.bottom);

	RECT rect = { l - 1, t - 1, r + 1, b + 1 };
	InvalidateRect(hWnd, &rect, 0);
}
