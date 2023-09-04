#include "RectTool.h"
#include "../../win/SimpleGraphics.h"

static HPEN iconPen = 0;

void RenderRectTool(RECT* frame, PaintContext* tool, HDC hdc, HWND hWnd)
{
	GET_NORMALIZED_RECT((*frame), rect);
	GET_EXPAND_RECT(__rect, toolRect, 0, 0, -1, -1);
	if (tool->isFill) {
		FillRect(hdc, &__toolRect, tool->brush);

	}
	else {
		DrawRect(hdc, &__toolRect, tool->pen);
	}
}

static void RenderIcon(RECT* rect, PaintContext* tool, HDC hdc, HWND hWnd)
{
	(HPEN)SelectObject(hdc, iconPen);
	int l = rect->left + 8; 
	int t = rect->top + 5;
	int r = rect->right - 8;
	int b = rect->bottom - 5;
	MoveToEx(hdc, l, t, NULL);
	LineTo(hdc, r, t);
	MoveToEx(hdc, r, t, NULL);
	LineTo(hdc, r, b);
	MoveToEx(hdc, r, b, NULL);
	LineTo(hdc, l, b);
	MoveToEx(hdc, l, b, NULL);
	LineTo(hdc, l, t - 1);
}

IPaintTool* CreateRectTool()
{
	iconPen = CreatePen(PS_SOLID, 2, 0);
	return CreateIPaintTool(RenderRectTool, RenderIcon, "RectTool");
}
