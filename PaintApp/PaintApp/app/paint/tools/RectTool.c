#include "RectTool.h"
#include "../../win/SimpleGraphics.h"

static HPEN iconPen = 0;

void RenderRectTool(RECT* frame, PaintContext* tool, HDC hdc, HWND hWnd)
{
	if (tool->isFill) {
		FillRect(hdc, frame, tool->brush);
	}
	else {
		if (tool->width > 1) {
			GET_NORMALIZED_RECT((*frame), normal);
			DrawRectInternal(hdc, &__normal, tool->pen, tool->width);
		}
		else {
			DrawRect(hdc, frame, tool->pen);
		}
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
