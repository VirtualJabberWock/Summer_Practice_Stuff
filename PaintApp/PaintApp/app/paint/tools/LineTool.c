#include "LineTool.h"

#include "../../win/SimpleGraphics.h"

static HPEN toolIconRectEdge = 0;

void RenderLineTool(RECT* frame, PaintContext* props, HDC hdc, HWND hWnd)
{

	int x1 = frame->left;
	int y1 = frame->top;
	int x2 = frame->right;
	int y2 = frame->bottom;

	char rb = (x2 >= x1 & y2 >= y1) * 1;
	char lb = (x1 >= x2 & y2 >= y1) * 1;
	char rt = (x2 >= x1 & y1 >= y2) * 1;
	char lt = (x1 >= x2 & y1 >= y2) * 1;

	x1 = x1 + rb + rt - lt - lb;
	y1 = y1 + rb + lb - rt - lt;
	x2 = x2 + lb + lt - rb - rt;
	y2 = y2 + lt + rt - rb - lb;

	DrawLine(hdc, x1, y1, x2, y2, props->pen);
}

static void RenderIcon(RECT* rect, PaintContext* tool, HDC hdc, HWND hWnd) {

	DrawLine(hdc, rect->left + 8, rect->top + 5, rect->right - 8, rect->bottom - 5, toolIconRectEdge);
}

IPaintTool* CreateLineTool()
{
	toolIconRectEdge = CreatePen(PS_SOLID, 2, 0);
	return CreateIPaintTool(RenderLineTool, RenderIcon, "LineTool");
}
