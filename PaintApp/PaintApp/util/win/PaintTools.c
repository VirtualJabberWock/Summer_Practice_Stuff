#include "PaintTools.h"
#include "SimpleGraphics.h"


void InitPaintToolsResourses()
{

}

void UpdatePaintTool(PaintTool* tool, int width, UINT color, char isFill)
{
	if (tool->pen != 0) DeleteObject(tool->pen);
	if (tool->brush != 0) DeleteObject(tool->brush);
	tool->width = width;
	tool->color = color;
	tool->pen = CreatePen(PS_SOLID, width, color);
	tool->brush = CreateSolidBrush(color);
	tool->isFill = isFill;
}

void RenderSquareTool(RECT* rect, PaintTool* tool, HDC hdc, HWND hWnd)
{
	GET_NORMALIZED_RECT((*rect), rect);
	GET_EXPAND_RECT(__rect, toolRect, 0, 0, -1, -1);
	if (tool->isFill) {
		FillRect(hdc, &__toolRect, tool->brush);
	
	}
	else {
		DrawRect(hdc, &__toolRect, tool->pen);
	}
}

void RenderLineTool(RECT* rect, PaintTool* tool, HDC hdc, HWND hWnd)
{

	int x1 = rect->left;
	int y1 = rect->top;
	int x2 = rect->right;
	int y2 = rect->bottom;

	char rb = (x2 >= x1 & y2 >= y1) * 1;
	char lb = (x1 >= x2 & y2 >= y1) * 1;
	char rt = (x2 >= x1 & y1 >= y2) * 1;
	char lt = (x1 >= x2 & y1 >= y2) * 1;

	x1 = x1 + rb + rt - lt - lb;
	y1 = y1 + rb + lb - rt - lt;
	x2 = x2 + lb + lt - rb - rt;
	y2 = y2 + lt + rt - rb - lb;

	DrawLine(hdc, x1, y1, x2, y2, tool->pen);
}
