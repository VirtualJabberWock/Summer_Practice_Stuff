#include "LineTool.h"

#include "../../win/SimpleGraphics.h"

static HPEN toolIconRectEdge = 0;

void RenderLineTool(RECT* frame, PaintContext* props, HDC hdc, HWND hWnd)
{
	DrawLine(
		hdc,
		frame->left, frame->top, frame->right, frame->bottom,
		props->pen
	);
}

static void RenderIcon(RECT* rect, PaintContext* tool, HDC hdc, HWND hWnd) 
{
	DrawLine(
		hdc,
		rect->left + 8, rect->top + 5, rect->right - 8, rect->bottom - 5, 
		toolIconRectEdge
	);
}

IPaintTool* CreateLineTool()
{
	toolIconRectEdge = CreatePen(PS_SOLID, 2, 0);
	return CreateIPaintTool(RenderLineTool, RenderIcon, "LineTool");
}
